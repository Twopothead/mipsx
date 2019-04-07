#include <stdio.h>
#include <inttypes.h>
typedef unsigned __int128 uint128_t;
#define get_bit(nr,value) ( ( (value>>nr) & 0x1) ? true:false )
void format_printb(int width,uint32_t value){
    printf("0b");
    for(int i=width-1;i>=0;i--){
            printf("%d",get_bit(i,value));
    }
}
// void format_printb64(uint64_t value){
//     printf("0b");
//     int width = 64;
//     for(int i=width-1;i>=0;i--){
//             printf("%d",get_bit(i,value));
//     }
// }
namespace EXP_Unsigned_Multiply{
    uint32_t Multiplicand;
    uint64_t Product;
    bool product_carry;//(The Product register should really be 65 bits to hold the carry out of the adder)
    uint32_t product_lo32;
    uint32_t product_hi32;
    void setMultiplyU(uint32_t multiplicand,uint32_t multiplier){
        Multiplicand = multiplicand;
        Product = 0 | multiplier;
    }
    uint64_t combine(uint32_t high32,uint32_t low32){
        uint64_t r;
        r = high32;
        r <<= 32;
        r |= low32; 
        return r;
    }
    bool calcu_product_carry(){
        return ((uint64_t)(Product>>32)+(uint64_t)Multiplicand) >> 32;
    }
    void format_printb64(uint64_t value){
        printf("0b");
        int width = 64;
        for(int i=width-1;i>=0;i--){
                printf("%d",get_bit(i,value));
        }
    }
    void MultiplyU(uint32_t multiplicand,uint32_t multiplier){
    // unsigned multiply
        setMultiplyU(multiplicand,multiplier);
        for(int i=0;i<32;i++){
            if(get_bit(0,Product)){
                product_lo32 = Product;
                product_hi32 = ((Product>>32)+Multiplicand);
                product_carry = calcu_product_carry();
                Product = combine(product_hi32,product_lo32);
            }
            Product>>=1;
            if(product_carry)
                Product |= ((uint64_t)1<<63);
            else Product &= ~((uint64_t)1<<63);
        }
        printf("%d * %d = %lld\n",multiplicand,multiplier,Product);
        // format_printb64(Product);
        // hex(0xfffffffe*0xffffffff) = '0xfffffffd00000002'
    }
}

namespace Booth_SignedMultiply{
// https://en.wikipedia.org/wiki/Booth's_multiplication_algorithm
// multiplies two signed binary numbers in two's complement notation.
    uint32_t m;// x = 32
    uint32_t r;// y = 32
    uint128_t A;// 65 bit x+y+1 = 65 
    uint128_t S;// 65 bit x+y+1 = 65
    uint128_t P;// 65 bit Product
    void setSigned_Multiply(uint32_t multiplicand,uint32_t multiplier){
        m = multiplicand;
        r = multiplier;
        A = (uint128_t)0 | m;
        A <<= 33;
        S = (uint128_t)0 | (int32_t)(-m);
        S <<= 33;
        P = (uint128_t)0 | r;
        P <<= 1;
    }
    void Signed_Multiply(uint32_t multiplicand,uint32_t multiplier){
        setSigned_Multiply(multiplicand,multiplier);
        for(int i=0;i<32;i++){
            switch ( (P & 0b11) ) 
            {
                case 0b01:
                    P = (P+A);
                    P &= ((((uint128_t)1<<64)) | (uint64_t)(-1));
                    break;
                case 0b10:
                    P = (P+S);
                    P &= ((((uint128_t)1<<64)) | (uint64_t)(-1));
                    break;
                case 0b00:
                    P = P;
                    break;
                case 0b11:
                    P = P;
                    break;
                default:
                    break;
            }
            P >>= 1;
        }
        P >>= 1;
        printf("%d * %d = %d\n",multiplicand,multiplier,P);
    }
    uint64_t get_product(){
        return (uint64_t)P;
    }
}
namespace EXP_DivideUnit{
// https://image.slidesharecdn.com/05-multiplydivide-140517040534-phpapp01/95/05-multiply-divide-19-1024.jpg?cb=1400299578
    uint128_t Remainder;// 65 bit
    uint32_t Divisor;
    #define get_bit(nr,value) ( ( (value>>nr) & 0x1) ? true:false )
    void setDivu(uint32_t dividend,uint32_t divisor){
        Remainder = dividend;
        Divisor = divisor;
    }
    uint128_t set_high32(uint128_t r,uint32_t high32){
        uint32_t origin_lo = r & 0xffffffff;
        r = high32;
        r <<= 32;
        r |= origin_lo; 
        return r;
    }
    uint128_t set_low32(uint128_t r,uint32_t low32){
        uint32_t origin_high = r >>32;
        r = origin_high;
        r <<= 32;
        r |= low32;
        return r;
    }
    uint32_t get_hi(){// Remainder
        return (Remainder>>32) & 0xffffffff;
    }
    uint32_t get_lo(){// Quotient
        return (Remainder & 0xffffffff);
    }
    void Divu(uint32_t dividend,uint32_t divisor){
        setDivu(dividend,divisor);
        uint64_t difference;
        for(int i=0;i<32;i++){
            Remainder <<= 1;
            difference = (Remainder>>32) - Divisor;
            if(get_bit(32,difference)==0){
                Remainder = set_high32(Remainder,difference);
                Remainder |= 0b1;
            }else{
                ;
            }    
        }
        printf("%d / %d = %d ... %d\n",dividend,divisor,get_lo(),get_hi());
    }
    uint32_t negate(uint32_t num){
        num = ~num + 1;
        return num;
    }
    void Div(uint32_t dividend,uint32_t divisor){
        bool s_dividend = get_bit(31,dividend);
        bool s_divisor = get_bit(31,divisor);
        bool s_remainder = (s_dividend) ^ (s_divisor); 
        Remainder = (s_dividend)?(-dividend):dividend;
        Divisor = (s_divisor)?(-divisor):divisor;
        uint64_t difference;
   
        for(int i=0;i<32;i++){
            Remainder <<= 1;
            difference = (Remainder>>32) - Divisor;
            if(get_bit(32,difference)==0){// carry out of the adder
                Remainder = set_high32(Remainder,difference);
                Remainder |= 0b1;
            }else{
                ;
            }    
        }
// Thus the correctly signed division algorithm 
// negates the quotient if the signs of the operands are opposite 
// and makes the sign of the nonzero remainder match the dividend.
        if(s_remainder){
            Remainder =  set_low32(Remainder, negate(Remainder & 0xffffffff));
        }
        if(get_hi()!=0)
            Remainder = set_high32(Remainder,(s_dividend ? negate(get_hi()):get_hi()) );

        printf("%d / %d = %d ... %d\n",dividend,divisor,get_lo(),get_hi());
    }
}

int main(){
    // EXP_MultiplyUnit::Multiply(0xfffffffe,0xffffffff);
    // EXP_MultiplyUnit::Multiply(8,9);

    // EXP_DivideUnit::Divu(74,5);
    // EXP_DivideUnit::Divu(56,0xffffffff);
    //EXP_DivideUnit::Divu(56,0xffffffff);
    EXP_DivideUnit::Div(74,-8);
    EXP_DivideUnit::Div(-7,-3);
    EXP_DivideUnit::Div(-7,3);
    EXP_DivideUnit::Div(-7,3);
    EXP_DivideUnit::Div(666,0);
    EXP_DivideUnit::Div(-666,0);
    EXP_DivideUnit::Div(0x80000000,0xffffffff);
    Booth_SignedMultiply::Signed_Multiply(9,-9);
    Booth_SignedMultiply::Signed_Multiply(0xfffffffe,0xffffffff);
    EXP_Unsigned_Multiply::MultiplyU(0xfffffffe,0xffffffff);
    EXP_Unsigned_Multiply::MultiplyU(8,9);
    return 0;
}