# Search-Pattern-Hidden

---

Implementation of Searchable Encryption Scheme with Search Pattern Hidden.

- KeyGen: implementation of the key generation algorithm in our scheme.
- Setup: implementation of the search index generation algorithm in our scheme.
- TGen: implementation of the search token generation algorithm in our scheme.
- Search: implementation of the search algorithm in our scheme.
- Decrypt: implementation of the decrypt algorithm in our scheme.

## Pre-requisites

---

gcc/g++, GMP(enable c++) and NTL

## Prepare Environment

---

See https://www.shoup.net/ntl/doc/tour-gmp.html for preparing NTL with GMP.

## Getting the code

---

```shell
$ git clone https://github.com/AuspiciousCloud/search-pattern-hidden
```

## Building

---

```shell
$ sh ./AutoCompile.sh
```

## KeyGen

parameter

Generate the keys in our scheme.

The system parameter is N, k, g, the public key is g^a, the key for the auxiliary server is p_prime, q_prime, and the key for the data owner is a.

---

## Setup

Generate the encrypted search index.

---

## Trapdoor

Generate the trapdoor. If we search a keyword w, we encrypt a random element. Otherwise, we encrypt the element $0$.

## Search

The cloud server performs search under the help of the auxiliary server.

---

## Decrypt

Decrypt the encrypted search results.

---
