# alu-blockchain

A small blockchain written in C: SHA-256 and ECDSA (secp256k1) crypto,
blocks with proof of work, UTXO transactions, and an interactive CLI.

## Layout

| Folder | What it is |
| --- | --- |
| `crypto/` | `libhblk_crypto.a`: SHA-256, EC key create/save/load, sign/verify |
| `llist/` | `libllist.a`: generic linked list used by the whole project |
| `blockchain/v0.1/` | Blocks, hashing, serialization |
| `blockchain/v0.2/` | Adds proof of work: difficulty, mining |
| `blockchain/v0.3/` | Adds transactions and unspent outputs (UTXO) |
| `cli/` | Interactive command-line program built on v0.3 |

## Requirements

- `gcc` and `make`
- OpenSSL development headers (`sudo apt install libssl-dev` on Ubuntu)

## Build

```sh
make          # builds every library and the CLI
make clean    # removes build output
```

Each folder also has its own Makefile, e.g. `make -C blockchain/v0.2`.

## Run the CLI

```sh
./cli/cli
```

Commands:

```
help [COMMAND]        list commands
info                  blocks, UTXOs, pool size, total coins
mine                  mine a block (pays 50 coins to your wallet)
send AMOUNT ADDRESS   send coins to a public key (130 hex characters)
save PATH             save the blockchain to a file
load PATH             load a blockchain from a file
wallet_save PATH      save your wallet keys to a folder
wallet_load PATH      load wallet keys from a folder
exit
```

Example session:

```
> mine
Successfully mined a block
> info
Blocks:  2
UTXOs:   1
TX Pool: 0
Coins:   50
> save chain.hblk
Saved blockchain to chain.hblk
```
