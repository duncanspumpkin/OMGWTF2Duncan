#include <stdio.h>

int curRand = 0;

int InitRand()
{
    //Optimised to use minimum amount of asm
    __asm{
        rdtsc;
        shr eax,1;
    }
}

int NewRand10()
{
    //Remember to replace the default values
    //with more random ones.
    //Should be faster now that asssembler is used.
    __asm{
        mov eax,curRand;
        mov edx,0x54632121;
        mul edx;
        add eax,54321;
        shr eax,3;
        mov curRand,eax
        and eax,1;
    }
}


extern "C"
{
    __declspec(dllexport) int ReturnRandInteger(int modulus)
    {
        //Init the random number generator.
        if (!curRand) curRand = InitRand();

        int newRand = 0;
        //NewRand10 returns a 1 or a 0 randomly.
        //To create a random number greater than that.
        //We can just call it 32 times and shift the result
        //and then mod the answer. To get the correct range.
        //Yes we could just use curRand but this will increase
        //entropy. I mean we call the function 31 times!
        for(int i = 0; i < 31; ++i){
            newRand |= NewRand10()<<i;
        }
        return newRand %= modulus;
    }
}



