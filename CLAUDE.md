# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

C++17 shared library (`libieee1905_1`) implementing IEEE 1905.1 CMDU/TLV handling, the transport layer of Wi-Fi EasyMesh. The specs are in `docs/` (`IEEESTD.2013.6502164.pdf` = IEEE 1905.1-2013, `Wi-Fi_EasyMesh_Specification_v6.0.pdf`). Check against them when you add message types or TLVs.

## Build

spdlog is a git submodule (`external/spdlog`). Initialize it before you configure:

```sh
git submodule update --init
mkdir build && cd build
cmake ..            # -DBUILD_EXAMPLES=OFF / -DBUILD_TESTS=OFF to skip examples or tests
make -j4
ctest --output-on-failure                       # run all tests
./tlv_test --gtest_filter=TlvTest.EmptyTlv      # run a single test
./simple_controller [iface]                     # example; defaults to eth0
```

API docs: `doxygen Doxyfile` (writes HTML into `docs/`).

## Tests

GoogleTest tests are in `tests/`. CMake uses the system GTest when available and otherwise downloads v1.14.0 via `FetchContent`, so the first configure may need network access. `TlvTest.SerializationWireFormat` pins the exact on-wire bytes; update it only if the encoding intentionally changes.

## Architecture

Everything is in namespace `ieee1905_1`. Public headers are in `include/ieee1905_1/`, and each header has a matching file in `src/`.

Data flow: `InterfaceManager` (raw socket) → `MessageParser::Parse` (bytes → `Cmdu`) → `Ieee1905Agent::HandleCmdu` (switch on message type → `Process*` handler). Sending goes the other way: build a `Cmdu` → `MessageSerializer::Pack` → `InterfaceManager::SendPacket`. Multicast goes to `01:80:C2:00:00:13`, and the EtherType is `0x893a`.

- **`Tlv`** (`tlv.h`) is a generic type plus a byte-vector value; it has no typed TLV classes. TLV type IDs are in `namespace TlvTypes`.
- **`Cmdu`** (`cmdu.h`) holds the header fields and a `std::vector<Tlv>`. `AddTlv` keeps an `END_OF_MESSAGE` TLV last when one is present. Message type IDs, including EasyMesh `0x8xxx` types up to R6, are in `namespace CmduTypes`.
- **`MessageSerializer` / `MessageParser`** are static big-endian codecs. The parser throws `std::runtime_error` on malformed input and requires the CMDU to end with `END_OF_MESSAGE`.
- **`Ieee1905Agent`** (`agent.cpp`, the largest file) holds `CmduTypeToString`, the `HandleCmdu` dispatch switch, `Send*` methods, and one `Process*` method per message type, grouped by 1905.1 vs. EasyMesh feature area. Most `Send*`/`Process*` bodies are logging stubs; only `SendTopologyDiscovery` builds a real packet.
- **`InterfaceManager`** is a stub: the socket code is commented out, `SendPacket` only logs, and `ReceivePacket` returns 0.
- **`LldpPdu`** (`lldp.h`) and **`WscMessage`** (`wsc.h`) are separate codecs for LLDP and WSC attributes.
- **Logging**: use the `EM_TRACE/DEBUG/INFO/WARN/ERROR/CRITICAL` macros from `logger.h` (a spdlog wrapper whose logger is initialized lazily).

### Adding a new CMDU type

You must update three places in sync: the constant in `CmduTypes` (`cmdu.h`), the string in `CmduTypeToString` (`agent.cpp`), and a `case` in `HandleCmdu` together with a `Process*` declaration in `agent.h` and its definition in `agent.cpp`.

### Wire format

Follows IEEE 1905.1-2013, with all multi-byte fields big-endian:
- CMDU header: 8 bytes = version (`0x00`), reserved, type (2), id (2), fragment id, flags (`CmduFlags::LAST_FRAGMENT` 0x80, `CmduFlags::RELAY` 0x40). `Cmdu` defaults to an unfragmented message (`LAST_FRAGMENT` set).
- TLV header: 3 bytes = 8-bit type (`TlvType` is `uint8_t`) + 16-bit length. The `Tlv` constructor throws `std::length_error` if the value is longer than 65535 bytes.
- `MessageSerializer::Pack` appends `END_OF_MESSAGE` if the last TLV isn't one, so callers don't need to add it. `MessageParser::Parse` ignores bytes after `END_OF_MESSAGE`, which is Ethernet minimum-frame padding.
- Fragmentation and reassembly are not implemented.

## Conventions

Methods are PascalCase, members use an `m_` prefix, files carry the Doxygen-style `/** file.cpp ... Author */` header, and include guards are `IEEE1905_1_<NAME>_H` (`logger.h` uses `#pragma once` instead).
