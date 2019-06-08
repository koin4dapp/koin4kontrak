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
Our decentralized random number generators(DRNG) bring fairness and tamper-resistance in the process of generating numbers to ensure fair game to all members. To Ensure the fairness our DRNG using 3 random number seeds. The first seed is generated from player transaction to DApp, the second one is from previous session seed saved at DApp, and the third is from the blockchain it self. So there are no Founder intervention, all is happened automatically at the blockchain based on DApp algorithm. Code snippet:

<code>
void init(uint64_t initseed) { //second seed=last session seed
  auto s = read_transaction(nullptr,0);
  char *tx = (char *)malloc(s);
  read_transaction(tx, s);
  capi_checksum256 result; //32bytes of 8 chunks of uint_32
  sha256(tx,s, &result); //First seed=player seed=fairness to player

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
  seed ^= seed ^= (initseed^(tapos_block_prefix()*tapos_block_num())); //Third seed=blockchain seed=fairness to all members
}
</code>

Information about read_transaction which we use as first seed(player seed) can be read at eosio transaction.h

<code>
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
</code>

Information about (tapos_block_prefix()*tapos_block_num() which we use as second seed(blockchain seed) can be read at eosio transaction.h

<code>
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
</code>

<h3>Pseudo Random Number Generator using SHA-256</h3>
The SHA-256 hash algorithm produces hash values that are hard to predict from the input. They are also roughly equidistributed as the input varies. The advantages of this approach for election auditing and some other applications include the following:
<ul>
<li>The SH-256 algorithm is public and many implementations are available in many languages.</li>
<li>Given the seed, anyone can verify that the sequence of numbers generated was correct—that it indeed comes from applying SHA-256</li>
<li>Unless the seed is known, the sequence of values generated is unpredictable (so the result is hard to "game"). It is very hard to distinguish the output from independent, uniformly distributed samples</li>
(Source: P.B. Stark, https://www.stat.berkeley.edu/~stark/Java/Html/sha256Rand.htm, last access 8 June 2019)

<code>
void randraw() { 
  capi_checksum256 result; //32bytes of 8 chunks of uint_32
  sha256((char *)&seed, sizeof(seed), &result); //distributed samples fairness to all members
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
</code>

<h3>Potential of Attack</h3>
The key of potential attack is in the 3 seeds. There are no way to attack outside the blockchain, but advanced attacker can make their own smart contract run on the blockchain to get tapos_block_prefix() and tapos_block_num() and must make sure that the time between the attacker smart contract and transaction initial by the smart contract must be run the same block, otherwise the two numbers will be changed and the result is different. But remember in game like dice, free dice dan lucky wheel result are based on value range (<50 or >51), so there are still have chance to attack. Code snipped to initial action to run transaction on our DApp from attacker smart contract:

<code>
auto seed3 = tapos_block_prefix() * tapos_block_num();
  ...
  ...
action(
  permission_level{_self, "active"_n,
  "targetcontract"_n, "targetaction"_n),
  std::make_tuple(_self, other params need by action)
.send(); 
</code>

(Source: https://bzdww.com/article/130403/, last access 8 June 2019)

Attack to session seed is possible even though the table are not added to the abi file, but advanced attacker can write smart contract to read it directly from the blockchain. They would typically create a struct and typedef similar to our table structure, and read it using our table code and scope, the code snipped:

</code>
struct similarobj {
  ...
}

typedef eosio::singleton <"targetobj"_n,similarobj> targetobj;

auto db = targetobj(targetcode,targetscope);
auto seed2 = db.get();
...

</code>

