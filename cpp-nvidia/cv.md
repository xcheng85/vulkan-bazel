# CV 

## video streaming

websocket

webrtc: Signaling, coturn server, stun/turn, p2p connection, 
session, icecandidates, peerconnection, turn webserver, 

## architecture 

### availability improvement
upstream service availability

decouple through message broker, fan out scaling

event push instead of polling

DDD

Domain event, 

### distributed transaction
2 phase commit


## software design and architecture
SOLID
single responsibility
Openess and closeness
Lis substitution
Interface segragation
Dependcy inversion


c++: boost di
golang: uber ioc 
node.js: inversify

## visualziation 

### cuda

Composite rendering with same geometry in openinventor

transform multiple volumes to the reference geomery using cuda streams

receive the binary tiles using grpc client: server streaming, 

### hightfield renderer
reader, client, rest client

gtest for mock up 

### performance measurement

load test + k6 + k8s concurrent jobs


### session agent

golang service binary to management rendering process

save gpu node consumption


## Graphics low-level api

1. cii to build sparse voxel octree from raw volumetric data
2. object pool pattern 
3. shader-based raycaster
4. brick upload
5. post-processing
6. entry exist prepass with depth test and wrote to texture

7. heightfield clipping: camera view render heightfield to depth texture and sample texture in the shader
