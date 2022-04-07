# Andrew File System (AFS)

## Port: 7001

## Proto: UDP

## Amplification factor: ~2x

## Reflector count: 16,500 (01/2020)

---

Andrew File System suffers from version information retrieval with unauthenticated connectionless udp query.

### Example Request / Response

- Request: 29 bytes

  - > ~# echo -ne '\x00\x00\x03\xe7\x00\x00\x00\x00\x00\x00\x00\x65\x00\x00\x00\x00\x00\x00\x00\x00\x0d\x05\x00\x00\x00\x00\x00\x00\x00'|nc -u 130.239.241.195 7001 -w1|wc -c

- Response: 93 bytes

  - ASCII: `�eOpenAFS 1.8.3-0ppa1~ubuntu16.04.1-debian 2019-12-03 @b-cn0835.hpc`
  - Raw Bytes: `000003e7000000000000006500000000000000000d040000000000004f70656e41465320312e382e332d30707061317e7562756e747531362e30342e312d64656269616e20323031392d31322d30332040622d636e303833352e687063`

### Documentation

- <https://zhu45.org/posts/2018/May/02/the-andrew-file-system-afs/>
- <https://www.youtube.com/watch?v=lS7FupJDFX0>
- <https://www.openafs.org/>

### Mitigations

- Stop allowing default Internet access (yet again...), restrict to LAN as default configuration (vendor issue)
- Response data ALWAYS consists of the query prefix `\00\00\03\e7` and the OpenAFS implementation will contain the string `OpenAFS`
