/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>
#include <math.h>
#include <random.hpp>
#include "eosio.setting.hpp"

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   class [[eosio::contract(OUR_CONTRACT)]] token : public contract {
      public:
         using contract::contract;

         [[eosio::action]]
         void create( asset  maximum_supply);
                      
         [[eosio::action]]
         void reset();
                      
         [[eosio::action]]
         void issue( string memo );
         
         [[eosio::action]]
         void retire( asset quantity, string memo );
         
         [[eosio::action]]
         void notify( name to, string memo );
         
         [[eosio::action]]
         void setenable( uint8_t play, uint8_t enabled);
         
         [[eosio::action]]
         void opensport(name event, uint64_t start_time, string match, uint8_t enabled,
            short o1x2_win, short o1x2_draw, short o1x2_away,
            short ah_home, short ah_away);
            
         [[eosio::action]]   
         void setsportstat(name event, uint8_t enabled);
         
         [[eosio::action]]
         void playsport(name participant, name event, asset so1x2_win, asset so1x2_draw, asset so1x2_away,
            asset sah_home, asset sah_away, asset sodd, asset seven);
            
         [[eosio::action]]
         void closesport(name event, uint8_t home, uint8_t away);            
            
         [[eosio::action]]
         void cleansport();             
         
         [[eosio::action]]
         void openltfour(uint64_t initseed);
         
         [[eosio::action]]
         void playltfour(name participant, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4 );
                    
         [[eosio::action]]
         void closeltfour(uint64_t initseed);    
         
         [[eosio::action]]
         void openltfive(uint64_t initseed);
         
         [[eosio::action]]
         void playltfive(name participant, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5 );
                    std::vector<name> win5;
         [[eosio::action]]
         void closeltfive(uint64_t initseed); 
         
         [[eosio::action]]
         void openltsix(uint64_t initseed);
         
         [[eosio::action]]
         void playltsix(name participant, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5, uint8_t n6 );
                    
         [[eosio::action]]
         void closeltsix(uint64_t initseed);          
         
         [[eosio::action]]
         void opentoto(uint64_t initseed);
         
         [[eosio::action]]
         void playtoto(name participant, asset quantity, uint8_t digit, uint16_t number);
                    
         [[eosio::action]]
         void closetoto(uint64_t initseed);
         
         [[eosio::action]]
         void openlottery(uint64_t initseed);
         
         [[eosio::action]]
         void playlottery( name participant, asset quantity);
                    
         [[eosio::action]]
         void closelottery(uint64_t initseed);
         
         [[eosio::action]]
         void playdice( name    player,
                      asset   quantity,
                      string  memo );
                      
         [[eosio::action]]
         void playfree( name    player,
                      asset   quantity,
                      string  memo );
                      
         [[eosio::action]]
         void playwheel( name    player,
                      asset   quantity );
                      
         [[eosio::action]]
         void transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo );
            
         [[eosio::action]]
         void cashierpay( name to, asset quantity, string memo);
                        
         [[eosio::action]]
         void open( name owner );

         [[eosio::action]]
         void close( name owner, const symbol& symbol );
         
         void ontransfer(eosio::name from, 
                        eosio::name to,
                        eosio::asset quantity,
                        std::string memo
         );
         
         [[eosio::action]]
         void withdraw( name    holder,
                      asset   quantity);
                      
         static asset get_supply( name token_contract_account, symbol_code sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         static asset get_balance( name token_contract_account, name owner, symbol_code sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }
         
         static name eosio_contract() { return name(MAIN_CONTRACT); }
         
         static symbol eosio_symbol() { return symbol(MAIN_KOIN,4); }
         
         static symbol our_symbol() { return symbol(OUR_KOIN,4); }
         
      private:
      
         struct [[eosio::table]] account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };
         
         struct [[eosio::table]] cpanel {
            uint8_t  play;
            uint8_t  enabled;
            asset    jackpot_prize;
            uint64_t close_time;
            uint64_t result;
            uint8_t  primary_key()const { return play; }
         };
         
         struct [[eosio::table]] play_session {
            uint8_t  play;
            uint64_t close_time;
            uint64_t last_time;
            uint64_t participants;
            asset    total_stake;
  
            uint8_t  primary_key()const { return play; }
         };
         
         struct [[eosio::table]] lottery {
            name     participant;
            uint64_t start_ticket;
            uint64_t end_ticket;
            
            uint64_t primary_key()const { return start_ticket; }
         };
         
         struct [[eosio::table]] toto {
            uint64_t key;
            name     participant;
            asset    quantity;
            uint8_t  digit;
            uint16_t number;
            
            
            uint64_t primary_key()const { return key; }
         };
         
         struct [[eosio::table]] lotto {
            uint64_t key;
            name     participant;
            asset    quantity;
            uint8_t  n1;
            uint8_t  n2;
            uint8_t  n3;
            uint8_t  n4;
            uint8_t  n5;
            uint8_t  n6;            
            
            uint64_t primary_key()const { return key; }
         };
         
         struct [[eosio::table]] sport_event {
            name     event;
            uint64_t start_time;
            string   match;
            uint8_t  enabled;//binary represent
            short    o1x2_win; //must divide 100
            short    o1x2_draw;
            short    o1x2_away;
            short    ah_home;
            short    ah_away;
            uint64_t settle_time;
            uint8_t  home_score;
            uint8_t  away_score;

            uint64_t primary_key()const { return event.value; }
         };
         
         struct [[eosio::table]] sport {
            uint64_t key;
            name     participant;
            uint64_t timestamp;
            asset    so1x2_win;
            asset    so1x2_draw;
            asset    so1x2_away;
            asset    sah_home;
            asset    sah_away;
            asset    sodd;
            asset    seven;  
            
  
            uint64_t primary_key()const { return key; }
         };         
         
         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;
         typedef eosio::multi_index< "cpanels"_n, cpanel > cpanels;
         typedef eosio::multi_index< "psessions"_n, play_session > psessions;         
         typedef eosio::multi_index< "lotteries"_n, lottery > lotteries;
         typedef eosio::multi_index< "totos"_n, toto > totos;
         typedef eosio::multi_index< "lottos"_n, lotto > lottos;
         typedef eosio::multi_index< "sevents"_n, sport_event > sevents;
         typedef eosio::multi_index< "sports"_n, sport > sports;
         

         void sub_balance( name owner, asset value );
         void add_balance( name owner, asset value, name ram_payer );
         
         void send_notify(name to, string memo);
         
         void send_action(name action_name, name participant, asset quantity, string memo);
         
         void set_deferred_tx(name action_name, uint64_t delay_sec, uint8_t play, uint64_t initseed);
         
         void openplay(uint8_t play, name autocloseaction, uint16_t closeseconds, uint64_t initseed, string message);
         
         void playlotto(uint8_t play, name participant, name lotto, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5, uint8_t n6, uint8_t m, uint8_t limit, string message);
         
         void calculate_EH(name participant, short odds, asset sodds, asset &profitlosstosupply, asset &totalfeetofounder, string message);
         
         void calculate_AH(name participant, double net_score, asset sah, asset &profitlosstosupply, asset &totalfeetofounder, string message);
         
         void calculate_OE(name participant, asset soe, asset &profitlosstosupply, asset &totalfeetofounder, string message);
         
         void closelotto(uint8_t play, uint64_t initseed,
              uint8_t maxball, uint8_t maxroll, uint8_t percent_of_prizepool[], uint64_t min_jackpot_prize,
              name lotto, string message, name open_action, uint16_t closeopentime);
         
         uint64_t seconds_before_midnight(uint16_t seconds) {
           return (rint(now()/86400)+1)*86400-seconds;
         }
         
         double exchange_rate(uint64_t adjust)
         {
          uint64_t supplyamt = get_supply(_self, our_symbol().code()).amount;
          return supplyamt<=0?EXCH_RATE:((supplyamt*1.0)/(get_balance(eosio_contract(), _self, eosio_symbol().code()).amount+adjust));
         }
         
         uint8_t is_enabled(uint8_t play)
         {
          cpanels cpanelstable( _self, _self.value );
          const auto& ct = cpanelstable.get( play, "play status not defined yet" );
          return ct.enabled;
         }
         
         void check_memo( string memo) {
          eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );
         }
         
         void check_asset( asset quantity, uint64_t minvalue, uint8_t nonfloat) {
          eosio_assert( quantity.symbol.is_valid(), "invalid symbol name" );
          eosio_assert( quantity.symbol == our_symbol(), "token symbol not match");
          eosio_assert( quantity.is_valid(), "invalid quantity");
          eosio_assert( quantity.amount > 0, "quantity must > 0");
          eosio_assert( quantity.amount >= minvalue, "quantity not reach minimum allowed");
          if (nonfloat > 0)
            eosio_assert( (quantity.amount%10000)==0, "quantity must integer");
         }
         
         void check_balance( account participant, asset quantity) {
          eosio_assert( participant.balance.amount >= quantity.amount, "not enough balance" );
         }
         
         string totonumber_tostring(uint8_t digit, uint16_t number) {
           string result = "";
           for (uint8_t i=0;i<digit;i++) {
             uint8_t remainder=number%10;
             result.insert(result.begin(),1,(char) (remainder+'0'));
             number /=10;
           }
           return result;
         }
   };

} /// namespace eosio