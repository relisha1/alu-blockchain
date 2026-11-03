# CLI

Command line interface for the blockchain in `blockchain/v0.3`.

## Build and run

```sh
make
./cli
```

## Commands

| Command | What it does |
| --- | --- |
| `help [COMMAND]` | List commands, or show help for one |
| `wallet_load PATH` | Load a wallet (EC key pair) from a folder |
| `wallet_save PATH` | Save the wallet to a folder (PEM files) |
| `send AMOUNT ADDRESS` | Create a transaction and add it to the local pool |
| `mine` | Mine a block with the pool transactions and a coinbase |
| `info` | Number of blocks, unspent outputs, pool size, total coins |
| `load PATH` | Load a blockchain from a file |
| `save PATH` | Save the blockchain to a file |
| `exit` | Quit |

## Extension: wallet commands

To make the blockchain usable as a small crypto-money, I added three
commands on top of the CLI:

| Command | What it does |
| --- | --- |
| `address` | Show your wallet's public key, so other people can send you coins |
| `balance` | Show how many coins your wallet owns (sum of your unspent outputs) |
| `print` | Show every block: index, difficulty, number of transactions, hash |

Files: `cli_address.c`, `cli_balance.c`, `cli_print.c`.

### Example: Alice pays Bob

Bob creates a wallet and gets his address:

```
> wallet_save ./bob
> address
04a3ea79707ff8c3...
```

Alice mines some coins and sends 30 to Bob:

```
> mine
Successfully mined a block
> mine
Successfully mined a block
> balance
Balance: 100 coins (in 2 unspent outputs)
> send 30 04a3ea79707ff8c3...
Transaction added to local transaction pool
> mine
Successfully mined a block
> balance
Balance: 120 coins (in 2 unspent outputs)
> print
Block 0 | difficulty 0 | 0 transaction(s)
  hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
    |
Block 1 | difficulty 0 | 1 transaction(s)
  ...
> save ./demo.hblk
```

Bob loads the blockchain with his wallet and sees the coins:

```
> wallet_load ./bob
> load ./demo.hblk
> balance
Balance: 30 coins (in 1 unspent outputs)
```
