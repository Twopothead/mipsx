#pragma once
#include <stdio.h>
#include <inttypes.h>
namespace Unsigned_Multiply{
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
    uint64_t get_product(){
        return Product;
    }
    #define get_bit(nr,value) ( ( (value>>nr) & 0x1) ? true:false )
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
        // printf("%d * %d = %lld\n",multiplicand,multiplier,Product);
        // format_printb64(Product);
        // hex(0xfffffffe*0xffffffff) = '0xfffffffd00000002'
    }
}
namespace Booth_SignedMultiply{
    typedef unsigned __int128 uint128_t;
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
        // printf("%d * %d = %d\n",multiplicand,multiplier,P);
    }
    uint64_t get_product(){
        return (uint64_t)P;
    }
}
namespace MultiplyUnit{
    uint64_t Multu(uint32_t multiplicand,uint32_t multiplier){
        Unsigned_Multiply::MultiplyU(multiplicand,multiplier);
        return Unsigned_Multiply::get_product();
    }
    uint64_t Mult(uint32_t multiplicand,uint32_t multiplier){
        Booth_SignedMultiply::Signed_Multiply(multiplicand,multiplier);
        return Booth_SignedMultiply::get_product();
    }

}