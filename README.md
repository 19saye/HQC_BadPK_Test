 (PQC_Prsch/BadPK)

Bu repo, araç sensör verisini post-kuantum kripto (PQC) KEM ile koruma
fikrini denemek için hazırlanmış küçük bir test ortamıdır.

Toy model bünyesinde iki ana kısım vardır:

 1) "mock" modu: tamamen deterministik ve eğitim amaçlı bir KEM modeli.
 2) "real" modu: PQClean içindeki HQC-128 (clean)  implementasyonunu kullanır.

Her iki modda da aynı “BadPK deneyi” yapılır:

Doğru public key ile türetilen shared secret ile,tek biti bozulmuş public key (`pk_bad`) ile türetilen shared secret’ı
karşılaştırıyoruz.Gerçek HQC-128’te bu, decaps tarafında hata  olarak ortaya çıkıyorr
(`dec(badpk) fail`).


## Derleme

### 1. Mock (eğitsel) KEM

```bash
make mock
./badpk
# HQC Bad Public Key Test
ÖRNEK ÇIKTI: 
PK=32 SK=32 CT=16 SS=32
PK selfcheck = EQUAL
baseline match = YES
badpk match = NO

''
baseline match = YES → normal akışta shared secret eşleşiyor.
badpk match = NO → tek bit bozulmuş public key, eşleşmeyi bozuyor.
''

Bu proje:

PQClean’in standartlanmış HQC-128 KEM implementasyonunu kullanır.
Kötü niyetli veya bozulmuş public key → kapsül açma (decaps) başarısızlığı test edilir.
Araç sensör verileri için hafif bir AEAD toy modeli kullanılır (8 byte veri + 2 byte tag).

Modüller:
mock/ → Eğitimsel KEM (basit deterministik XOR türevi)
real/ → PQClean HQC-128 (IND-CCA2 güvenlik düzeyi)
scripts/ → log karşılaştırma & analiz

PK=2249 SK=2305 CT=4433 SS=64
baseline match = YES
dec(badpk) fail
/// bu örnek derlemesi:
make clean
make mock   # toy model
make real   # HQC-128 gerçek KEM

“Bad Public Key Attack Surface” test frameworkü, Multi-KEM genişlemeye hazır mimari (HQC, Kyber, Saber…)
AEAD + KEM kombinasyonu ile hafif araç telemetrsi için kuantum-dayanıklı kanal
yeniliklerine sahiptir. 
