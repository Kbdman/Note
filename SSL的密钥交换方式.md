# RSA
Client通过ServerCertificate获取Server的证书以及公钥    
Client生成预主密钥，通过ClientKeyExchange将生成的预主密钥发送给Server   
Server使用私钥解密，获得预主密钥   
双方根据预主密钥获得加密传输用的对称密钥    

# DHE
q和p是全世界公认的一对值       
Client计算取一随机数Xa,计算出Pa = q^Xa mod p,将Pa发送给Server   
Server取一随机数Xb,计算出Pb = q^ Xb mod p,将Pb发送给client    
算法保证 Pb^Xa mod p 和Pa^Xb mod p 可以计算出统一的结果，作为加密传输的对称密钥    
>在DHE中，服务端证书和公钥仅用于服务端认证，不参与到密钥交换

# ECDHE
Q(x, y)作为公认的某个椭圆上的点
客户端生成值Ra ,计算 Pa(x, y) = Ra * Q(x, y)，得出Pa(x,y),将Pa(x,y)发送给服务端
服务端生成值Rb ,计算 Pb(x, y) = Rb * Q(x, y)，得出Pa(x,y),将Pb(x,y)发送给客户端   
算法保证 Ra * Pb(x, y)，Rb * Pa(x,y)可以计算得出相同的值S，S作为预主密钥    
ECDHE和DHE的流程类似，只是采用的算法不一样

## ECDHE和ECDH的区别
ECDH Server端不使用临时生成的Pb，而是使用证书公钥作为Pb,私钥作为Rb,因此ECDH必须使用ECC证书
