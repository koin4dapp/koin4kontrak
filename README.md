<h1>Player to Player Community Token</h1>
KOIN is DApp run on EOS block-chain to facilitate Player to Player interaction.

<h2>True Value Offering & Better than Any Offering</h2>
This is not an ICO activity, but an opportunity sharing offering. Members and Founder share the same value result, no free token distribute to Founder, all KOIN hold by founder came from his own fund and convert with same rate as other member.

Every EOS token collected is save securely at KOIN smart contract, and can be withdraw at any time. Deposit and Withdraw not depend on any Exchange; all functions are integrated in KOIN smart contract. Price is calculating based on current supply divide by total EOS in smart contract account.

Value result from member activity will distribute to all member immediately in term of token supply, which is directly impact to token price (Price=Current Supply/Total EOS).

Value sharing is come from percentage of surplus or deficit in each game session. In Dice, Free Dice, Free Wheel, TOTO, and Soccer, value sharing is 100% from surplus/deficit. In Lotto4x20, Lotto5x36, Lotto6x45, value sharing is 30% from surplus and 70% will go to jackpot prize pool. Surplus value will reduce token supply and Deficit value (after deducted from jackpot prize pool) will increase token supply.

Koin DApp   -   Founder

visit us: <a href="https://koin4dapp.appspot.com"> koin4dapp</a>

<h1>3 Seeds Random Number Generator (3SRNG)</h1>
Our decentralized random number generators(DRNG) bring fairness and tamper-resistance in the process of generating numbers to ensure fair game to all members. To Ensure the fairness our DRNG using 3 random number seeds. The first seed is from previous session seed saved at our DApp + secret formula, the second seed is generated from current transaction ID, and the third seed is currently executing transaction block number. So there are no Founder intervention, and all is happened automatically at the blockchain based on DApp algorithm. The code snippet:

```
void init(uint64_t initseed) { //first seed=previous session seed saved at DApp + secret formula
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

<h3>Possibility of Parameters Attack</h3>
KOIN token is player to player community DApp, so that we must make sure that is secure from the attacker which can cause loss to all KOIN token holder, but because the execution of a smart contract in blockchain must be deterministic to keep the consensus, the attackers can take benefit from this behaviour.

To attack our last session seed, attacker would typically create a struct and typedef similar to our singleton table and read it using the same code and scope as our DApp. The code snipped to read value from a table that are not added to the ABI file is:

```
struct similarobj {
  ...
}

typedef eosio::singleton <"targetobj"_n,similarobj> targetobj;

auto db = targetobj("targetcode"_n,name("targetscope").value);
auto seed1 = db.get();
...
```

To attack transaction Id, at first we must understand how eosio generate transaction ID. The transaction ID is generated using a hash of current packed_trx in memory. The code snipped to get transaction ID in a smart contract is:

```
auto s = read_transaction(nullptr,0);
char *tx = (char *)malloc(s);
read_transaction(tx, s);
printhex(tx,s); //packed_trx
capi_checksum256 result; //32bytes of 8 chunks of uint_32
sha256(tx,s, &result);
printhex(&result, sizeof(result)); //transaction ID
```

If a smart contract issue several inline synchronous execution actions, then all of the action will execute within the same transaction as the original action and all actions in a transaction will execute in a same block, so they share the same parameter values. The code snipped is:

```
auto s = read_transaction(nullptr,0);
char *tx = (char *)malloc(s);
read_transaction(tx, s);
printhex(tx,s); //packed_trx
capi_checksum256 seed2; //32bytes of 8 chunks of uint_32
sha256(tx,s, &seed2);
printhex(&seed2, sizeof(seed2)); //transaction ID

auto seed3 = tapos_block_prefix() * tapos_block_num();
  ...
  ...
action(
  permission_level{_self, "active"_n,
  "targetcontract"_n, "targetaction"_n),
  std::make_tuple(_self, other params need by action)
.send(); //the target action will have same transaction ID, tapos_block_perfix() and tapos_block_num()
```

<h1>Possibility of Rollback Attack</h1>
In our DApp process, we have to make sure avoiding Rollback Attack. In the rollback attack technique, the attacker can make a smart contract that issue several inline synchronous execution actions, then compare  the balance before and after execution of our smart contract, and issue a rollback if the ending balance < the begining balance. The code snipped is:

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
<h1>Decompiler Issue</h1>
Smart contract wasm code can be download using /v1/chain/get_raw_code_and_abi RPC Api call.

```
<html>
<script>
function download(base64encoded, fileName, contentType) {
    var a = document.createElement("a");
    var file = new Blob([base64encoded], {type: contentType});
    a.href = URL.createObjectURL(file);
    a.download = fileName;
    a.click();
}

var data = "{\"account_name\":\"targetkontrak\"}";

var xhr = new XMLHttpRequest();

xhr.addEventListener("readystatechange", function () {
  if (this.readyState === this.DONE) {
    console.log(this.responseText);
 	download(JSON.parse(this.responseText).wasm,'b64wasm.txt',"data:application/octet-stream");
	download(JSON.parse(this.responseText).abi,'b64abi.txt',"data:plain/text,");
  }
});

xhr.open("POST", "https://api.bossweden.org/v1/chain/get_raw_code_and_abi");
xhr.setRequestHeader("accept", "application/json");
xhr.setRequestHeader("content-type", "application/json");

xhr.send(data);
</script>
</html>
```

Then the base64 encoded wasm can be convert to wasm binary file, and can be decompiler from bytecode to opcode using decompiler tools. Online base 64 decode available online at https://www.base64decode.org/ and Online decompiler available online at https://wasdk.github.io/wasmcodeexplorer/.

<h1>Attack Prevention</h1>
It is impossible to create a pure secret random number if we must revealed all our code as open source , because the behaviour of smart contracts must be deterministic to keep the consensus. The attacker can duplicate our code, try to calculate the result and transfer it to our smart contract via action call. To overcome this we have kept part of our code close source as secret formula.

In eosio we can run series of actions as a transaction, and if one the action issue a rollback, then all executed action in the transaction will rollback. To prevent this behaviour we put parts of our code as deffered action (delayed > 0.5 seconds) which is run at separate transaction and not effected by the rollback.

All smart contracts suffered from Decompiler approach, and currently no way out.

<h1>Conclusion</h1>
So far, we can make a conclusion that our DApp are secure and we ensure fairness to all members. If there are possible to attack our DApp, it is hard to do, even though they can try to Decompiler our smart contract wasm raw code.
