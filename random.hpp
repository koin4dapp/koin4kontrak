#include <eosio.token/eosio.setting.hpp>
#include <eosiolib/transaction.h>
#include <string>

class random {
private:
    uint64_t seed;
    std::string balls;

public:

    uint8_t rolls[6] = {0,0,0,0,0,0};

    void init(uint64_t initseed) { //initseed = previous session seed
        auto s = read_transaction(nullptr,0);
        char *tx = (char *)malloc(s);
        read_transaction(tx, s);
        capi_checksum256 result; //32bytes of 8 chunks of uint_32
        sha256(tx,s, &result); //player action seed

        seed = result.hash[7];
        seed <<= 8;
        seed ^= result.hash[6];
        seed <<= 8;
        seed ^= result.hash[5];
        seed <<= 8;
        seed ^= result.hash[4];
        seed <<= 8;
        seed ^= result.hash[3];
        seed <<= 8;
        seed ^= result.hash[2];
        seed <<= 8;
        seed ^= result.hash[1];
        seed <<= 8;
        seed ^= result.hash[0];
        seed ^= seed ^= (initseed^(tapos_block_prefix()*tapos_block_num())); //blockchain seed
        
        balls = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWMXY[\\";
        
#if DEBUG        
        eosio::print("seed:",seed,"\n");       
#endif         
    }
    
    void randraw() { 
        capi_checksum256 result; //32bytes of 8 chunks of uint_32
        sha256((char *)&seed, sizeof(seed), &result);
        seed = result.hash[7];
        seed <<= 8;
        seed ^= result.hash[6];
        seed <<= 8;
        seed ^= result.hash[5];
        seed <<= 8;
        seed ^= result.hash[4];
        seed <<= 8;
        seed ^= result.hash[3];
        seed <<= 8;
        seed ^= result.hash[2];
        seed <<= 8;
        seed ^= result.hash[1];
        seed <<= 8;
        seed ^= result.hash[0];
        
#if DEBUG        
        eosio::print("next seed:",seed,"\n");       
#endif 
    }
    
    uint64_t getseed() {
      return (seed>>8);
    }
    
    uint32_t rand(uint32_t to) { //generate random 1 - to
        randraw();
#if TESTING        
        return (uint32_t)((8888888888 % to) + 1);        
#else        
        return (uint32_t)((seed % to) + 1);
#endif        
    }
    
    void ballrolls(uint8_t to) {
        randraw();
        uint8_t i=0;
        uint64_t results = seed;
        while (i<6) {
          uint8_t ball = (uint8_t)(results%(to-i));
          rolls[i] = balls.at(ball)-'0'+1;
          balls.erase(ball,1);
          results >>= 7;
          i++;
        }
#if TESTING        
        rolls[0]=1;
        rolls[1]=2;
        rolls[2]=3;
        rolls[3]=4;
        rolls[4]=5;
        rolls[5]=6;        
#endif
    }
    
    std::string rolls_tostring(uint8_t n, uint8_t m) {
        std::string result="";
        uint8_t i = 0;
        while (i<m) {
          if (i<n) {
            if (i< m-1)
              result = "-" + to_string(rolls[i]) + result;
            else
              result = to_string(rolls[i]) + result;
          }
          else
          {
            if (i< m-1)
              result = "-X" + result;
            else
              result = "X" + result;            
          }
          i++;
        }
        return result;
    }
    
    uint64_t rolls_touint64(uint8_t m) {
      uint64_t result=0;
      for (int i=m-1;i>=0;i--) {
        result*= 100;
        result+=rolls[i];
      }
      return result;
    }
    
    uint8_t rolls_count(uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5, uint8_t n6, uint8_t m )
    {
      uint8_t result=0;
      uint8_t n[6] = {n1,n2,n3,n4,n5,n6};
      for (uint8_t i=0;i<m;i++) {
        if (n[i]==rolls[i])
          result++;
        else
          return result;
      }
      return result;
    }
    
    //expected value = 980
    int wheelcarlo(uint32_t rnd_1_1000) {
        if (rnd_1_1000 < 539)
          return 0;      
        if (rnd_1_1000 < 990)
          return 2;
        if (rnd_1_1000 < 994)
          return 5;
        if (rnd_1_1000 < 997)
          return 6;
        if (rnd_1_1000 < 999)
          return 10;
        return 20;
    }
};
