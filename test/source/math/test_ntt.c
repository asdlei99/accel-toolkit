#include "../../../math/ntt/ntt.h"
#include <stdio.h>
#include <memory.h>
#include <x86intrin.h>

static const uint64_t Prime = 0x7fffffffffffbc01;
static const uint64_t PrimitiveRoot = 0x0000000000000007;
static const uint64_t X[512] = {
        0x44467b09cfc9665b, 0x14c09f00e2424b6e, 0x51adf6204808e3bb, 0x2e6b677a304cd5ff,
        0x090987e4edb87c37, 0x572f20ce38bb56ae, 0x0e3b807a366b561a, 0x1933d3442f4ac29d,
        0x13bb4d9b8b892f51, 0x14dc06d216ffc3cf, 0x491fdb540b68e728, 0x50ff5dc0821932d0,
        0x5521540fe0c9cbc2, 0x437aeeb5273062fc, 0x18ec8290030f9c0f, 0x5d5387911316c4da,
        0x4a9bcd1c98635754, 0x6d01593585c44a48, 0x02a3b73e4181b11e, 0x6bacf168f91ee55a,
        0x7545d6729d60f501, 0x1bde354ad81062a3, 0x0c9e990f7027ba97, 0x7b7d54347ae09de4,
        0x76c92cb00aab0bd8, 0x2e5b0eb4bcb68f96, 0x5f70208582f4f3a8, 0x409fe49e9cc8911c,
        0x1752a69774e47d72, 0x50ce7bbab61e3100, 0x73a7022a20185d23, 0x019bf665b17bcae7,
        0x43686fc72f14c69d, 0x0b8381529ad2ba31, 0x752895e40f7312d5, 0x2b40844820553099,
        0x4d371d2f364aa715, 0x2bc28870c527c4ef, 0x19d074eb465c97f4, 0x5420cbf54c85c657,
        0x703d1f04b6402200, 0x7dd3c59f2ce14d8e, 0x126e250fe2fdedc5, 0x5cb6fb2a5d5b30e5,
        0x5d1ade20723e66dd, 0x4289d40c373f6653, 0x03edcc3904d33f01, 0x69cad3f368a15aa2,
        0x4722ff8f6c23c422, 0x5a70572e7fe43b52, 0x17f75f10e7dedb65, 0x4e544addd454169e,
        0x25eb4446e7893131, 0x561261c81006ef16, 0x3762001c3d946495, 0x1d42d62aaf28d2aa,
        0x4a696a3119fb9545, 0x32daba39d83a8404, 0x0213ec8a79b4e2b0, 0x7e1682f0956943d8,
        0x4aac0ca589639e70, 0x28d8a6bf24273cc9, 0x0e9d2fa5825632c3, 0x18e012bbec5b4288,
        0x25a34c17ff9df835, 0x583f8990421abd54, 0x3c387fcb8adac972, 0x183e6492147227ac,
        0x1cbbaf5d77b4d7ed, 0x3d0d40129c556fc4, 0x7b0b5dabe91723d2, 0x0272f789f4effc38,
        0x0289a81a1acad9a4, 0x62549da423732457, 0x048827fd6c55c416, 0x0e52634d7dcd4f2e,
        0x04fbfa21b3b51048, 0x04d619851737fcf7, 0x3e3114b5172f480c, 0x1eb6fd2f468e8c2b,
        0x3dfa821321ddf0d5, 0x0a70e673a5d47e5a, 0x4ba4820ba2b15d3a, 0x5f8caba21fa7508a,
        0x03fef640ba7b0488, 0x255880f4c03fcdee, 0x26f5977f7ffde35f, 0x7ec26b953eb91cc7,
        0x333ecf589777480a, 0x18dc8d61a28122c1, 0x19846a4a3df0e08c, 0x3e1c13a154c4509a,
        0x5857f96dc1b4a8e4, 0x6f9cd6a0011f7050, 0x2ab276875f20430f, 0x168158e7f67611cc,
        0x4ac75f396cef5d29, 0x7af45d397237b013, 0x62470c8035577cdd, 0x34d8042d58ba47d5,
        0x031cf7e308557d93, 0x0fcc6e10aac61ca7, 0x06bce59be57c9218, 0x585189ba86335c69,
        0x19f18c39596c6121, 0x3c4bfb76b1adc538, 0x1834ef8b668f5931, 0x1166f6b1b81d340f,
        0x63e0400426685099, 0x5702d614e61d6252, 0x4cfb1b1d62206e2f, 0x11004dffe1af1d55,
        0x3333a088fe17ffe4, 0x535d9163333758b2, 0x6f3fcc952d143bb4, 0x6185868cf0bdc1b5,
        0x05d3b24068bf8d36, 0x5a1e8adfcdb2166e, 0x45aab540ec9671d0, 0x5f48beb8e32f20a9,
        0x4ef59f05a7a69311, 0x4e3f428e77829c90, 0x508b4b46f65708f1, 0x2f9108e8e1204542,
        0x3d9aa74079758301, 0x752555e9c6eb8ed3, 0x6bed4f3e1f538577, 0x79d29b65ac69a49d,
        0x4139cd80ec549907, 0x4eb1f388fc8660d5, 0x6da7e23d75f6c7b3, 0x36f1da7d0094a400,
        0x02a1fbc61ea12b88, 0x027fb55f47ae9c4c, 0x37ffb619c6eb3761, 0x435e3c600fe9c7de,
        0x1908c1a1f44d8dc9, 0x642153a0a726a252, 0x1ff6a88828c6a208, 0x19524cee7ec0f598,
        0x4e619f5d8482c29a, 0x10e807cbdda32ca4, 0x4c1ae7a2f660a35d, 0x7f5ed9dca7baa1a5,
        0x150ff2a77479f14b, 0x3b72cb1a50a7bb38, 0x41ba44c3848e7e0f, 0x443bbec2b9530543,
        0x1d6b5933234b822e, 0x10637b756a0c8a01, 0x2038e36d2d96f29d, 0x04edefa2a6ace87d,
        0x3df1cac8f7a0a309, 0x4621545a51e722d2, 0x3055942924a95b81, 0x38d2b609c17f3c67,
        0x541aba001a917cf9, 0x459fe0618d5b10c8, 0x41a90ce2f742ae8d, 0x304165cea71e5943,
        0x43aa273c45ea1dca, 0x4308cfba5e206913, 0x5762caea5e6c85e3, 0x62f2850300703bb4,
        0x112cc7bb0cf0f558, 0x1cea8fa8699f5529, 0x57b033011e6ffd79, 0x3439771cbb713fad,
        0x64ffc0f004ba2a06, 0x6b7e2c043437ec7c, 0x78ff382f63413344, 0x62d488f2ac7ed246,
        0x121a773954f441d9, 0x3f13e0e47fb0d158, 0x40873691d3c9833d, 0x2596414f32126a31,
        0x046c2c0d2bed2db6, 0x56603a16bbba5068, 0x648a40a8a594c2aa, 0x4d05896691880dd3,
        0x44edf83efdfe55d7, 0x36be7b208de4f88b, 0x695a43c193c4c6d4, 0x20f23445d51eea75,
        0x22ca0e811ab929c3, 0x12cb5c2907103d55, 0x6a30f07e1167bdac, 0x43ea96275a264571,
        0x2703467593b4b9c8, 0x0aa4ddfe2e5201ba, 0x489a92a2dd88d657, 0x62944e39f839c111,
        0x582f843242615107, 0x52616d2c4998997c, 0x572e142e15ee3e16, 0x59efc6d95c525c7f,
        0x6054e10b08cb0099, 0x2640958f41dd1147, 0x37bb82f30814ef5e, 0x782583efa4afd118,
        0x22c4bb1c65d3c7fe, 0x11e68a1d5ed6104d, 0x2bfe0a0f7e0302f3, 0x349aec9359e0785a,
        0x1c103793d0ba2c29, 0x1c56b4a7e096a47c, 0x6213fbb72aa6f4dd, 0x42bc6437af5157d3,
        0x659dda3ee1757568, 0x75d1d13cc08f4769, 0x18312661bac80cae, 0x6cdee29d1e2cddc9,
        0x3b8a0c6ba0cc5b20, 0x143a390a94603795, 0x50374f8ac9569986, 0x164f62a0c84e1a47,
        0x0ec87ea942f79f44, 0x00ce1a5519d5f47a, 0x6960643b33004672, 0x00a8d80a58409b39,
        0x57ecf03f97acbbb8, 0x1bfd60606c9542d6, 0x26ef4d21ddd774e4, 0x43bb57bc5ad1ddee,
        0x55e75ae610a6fec7, 0x46fe5fcdbd6e2bfe, 0x0176bc590caee5e4, 0x15e9efe18d5ed2c0,
        0x7e41583848512f87, 0x361c51689d057627, 0x0cd0f6ce5be5d243, 0x436c475ae0d46689,
        0x102505b161ef39ea, 0x29b80b576b483cad, 0x1aa17616d3f38b93, 0x5a38f8415bd29d55,
        0x7a255efaafcd8b65, 0x657e0050e6e6fb83, 0x5a759e7eacd7ff68, 0x05debc01b568f668,
        0x045ed53522e015e9, 0x15fa18a4b6d50f47, 0x34a034bba58d6c8b, 0x5df5e05ab8718399,
        0x15081c52c5df7de0, 0x5b745e8d28e406db, 0x55f3ca5201d8a908, 0x4725fb518f589a2f,
        0x6527aaa2b8e20802, 0x309b94643332050c, 0x5cc59045717c7990, 0x767b33777bac614c,
        0x2639e46d45f7f4b1, 0x138acc14b420f280, 0x409bd316f9004a43, 0x1b2265bd4757b495,
        0x49352339718d0122, 0x35703935d654bb41, 0x174ddfdb7c636474, 0x46c08242fe96a544,
        0x5ba8a1e912b1841e, 0x2c4b33beb7fa70a2, 0x18366a778ec4d37b, 0x32f72ea62ecf9d04,
        0x67d3f70790535289, 0x39c1616d6df5203c, 0x1073c236c9542a16, 0x5634de1b5a95158f,
        0x0883e60f46622969, 0x1fc568916c19c198, 0x2ef68075fc499fe2, 0x2ded666de572120e,
        0x439c29f0c1c2d8ce, 0x28286a92c4c2870e, 0x06e93f8f81b1147c, 0x48df5d6fb23197c7,
        0x5823ee6c7bba83b8, 0x3f3d322b5810e723, 0x492dc33d22632b8d, 0x511f928861ed6f68,
        0x5c896b2ae1fcb544, 0x222decc47ff5bd8e, 0x0f54c0f8f526bb81, 0x30f0ed3fd8a173c8,
        0x5f618f987e14d743, 0x25e75f9c6c31d6c9, 0x1aff3317a44965d2, 0x47032e04aaeb378a,
        0x59384f0d23fbacb4, 0x040865a1af14f836, 0x21e497994f7aca59, 0x1cc60d768462c09d,
        0x2f03478afbdfec74, 0x65f7125031eaffa4, 0x3aaedbd5b00c8737, 0x17e7d8d5e7ce1f27,
        0x628fbe53926ab25c, 0x2131374141f2298b, 0x31741397aadde4a5, 0x78e1d4ed682b6c9d,
        0x38d10fe512485065, 0x241893a8ff180597, 0x255e7e9e847c5b99, 0x63a7f4c61276fe11,
        0x1ebd7d476fa74c21, 0x6ba25ffa0274c3ab, 0x3dfa626f2ad7ce6b, 0x492cfcadcff30d18,
        0x53a65bdefcfcb03b, 0x35c27f2b68bc10a9, 0x7103a54bf999e898, 0x0306362fdf9e300f,
        0x7a0df0ceb11c8550, 0x71acd003bac868cb, 0x2561f7746443cf92, 0x0b449d07a49d4862,
        0x538978efe5bc4785, 0x1345f1c816b52084, 0x319466514befaf28, 0x11c27a8ca616d194,
        0x6af9feaf0d408978, 0x5373dc5c9b3b4e54, 0x64a2ffa00dc16c6e, 0x7cd5159b17e9e89a,
        0x430450be7d3ca6cc, 0x4cd1833bdd741825, 0x00e76aa2a2d3b5aa, 0x1f6ebae86abb6155,
        0x51cc737ff87d586e, 0x5ae22a474c47b362, 0x12f5fe7cdc46b9c6, 0x309c1bf925ddf359,
        0x695b21128b3704b6, 0x6e209928dcd3060c, 0x3b7da15d9972705b, 0x239a2eacc4c677f2,
        0x0eb3b69f28710b24, 0x517f77bb965d4994, 0x2485571caaedd8a6, 0x1d6d20f1beff8fe9,
        0x7c81b2ac476b61bb, 0x50d15abb031c4a34, 0x60f962b2668d6d7e, 0x6df34b1458476efc,
        0x69442c50a72401ca, 0x168bf0b5636fd0e2, 0x776415623561da63, 0x4715fa293a63669b,
        0x186f1461c867085f, 0x215c194e204ecb78, 0x0ab59659d222fd7d, 0x7213bdd0ebb0bdc3,
        0x23a9ea93b2274494, 0x761eb1af3ab9998e, 0x08032aa24e43470d, 0x2d92b35a04b60760,
        0x6afd44e5496c9a38, 0x3cdd5430bb8dcabb, 0x71e6f12c2a004305, 0x1dd6b56f6343ac03,
        0x0d9ae3236c5c0b5e, 0x0435233a9a7d994d, 0x30badc5b447d23b0, 0x0ae2d69d604f045c,
        0x166fb9d03ccdc3de, 0x37847e39d9b3a282, 0x303bee5cb800e36d, 0x2dc3caae9cf00fc5,
        0x00133b7398ea94bb, 0x1fdd7e24a0d6c28a, 0x6696e1a4310c947a, 0x7d4662ddcb66d154,
        0x25a2ac20b7429495, 0x74c85462d8a6f3df, 0x6b4d9a7fec44850d, 0x0924fc0bc999e0b7,
        0x29f1c6a62dae1caf, 0x28541101a992fff9, 0x1f27d5a25a921ced, 0x3dd994a65cd9992a,
        0x0e89b0a06bbb29b2, 0x397806aedd25bfc5, 0x3e05c202c61e9698, 0x31bddfeb49f4fdbb,
        0x61fcdaf87320474e, 0x6f04ac1d029c0c3f, 0x3429265a638ae4f7, 0x6da1c298c49314fe,
        0x78c3c02fb5625926, 0x471735f832a7b5d8, 0x3ad601b4f427cff3, 0x1b81982991077d71,
        0x28fbd284155df6ca, 0x3295dc62d86ea069, 0x7c7db8d4e723f258, 0x3670bc9bd4e3ebac,
        0x20b33e35a5916be6, 0x4b2d272596666399, 0x2dee0f1bb79bf192, 0x335aa517ca64053d,
        0x12a7f9b2febca136, 0x186df2b8b22efaa0, 0x1d6d0b9c2eb152cb, 0x75ad9a15c2f0db69,
        0x18b40001d15a981e, 0x25de064efcc02251, 0x5f0a26045106c3fc, 0x745e1d7669755894,
        0x3182fac605915036, 0x59ef241027fcdf9d, 0x2181a75ba75f6f65, 0x21360ccb31697aea,
        0x572467bd6f475054, 0x24f12187f200e485, 0x4a40ab82f9afd905, 0x1df0b81d7ec91ce7,
        0x48fe4e57b5ef66d7, 0x6da4414164eb9cf1, 0x0abdb4b405df03c0, 0x6636445b0d5f04bd,
        0x3d79d7a120fa793b, 0x62963406e4d9b4b0, 0x09b11893268c1017, 0x27e4eaccdbc0b24c,
        0x02000b2543071d32, 0x27d1bd79220dbb9b, 0x52328811b32b149e, 0x2ab26385ea00df3f,
        0x3537bdd8fdaab584, 0x65df8aee565b0bec, 0x338401347cb429b5, 0x3fdbe657f6ff0368,
        0x62790148011b889d, 0x7f3b4c15b8c3dacb, 0x2b3158e3b8445b9c, 0x49aba93232d10567,
        0x6d028a2824a49dd5, 0x401e0fbbb9a2b5d0, 0x2721b986f8cec286, 0x3419abe9edb18856,
        0x20be76dfc6e721f0, 0x3d76f66dfad99dd1, 0x18d11dac2ade11a7, 0x6683dc2e55b7767b,
        0x742f3b52838743ad, 0x2f8e7b3150b7c744, 0x50d251056ee051c6, 0x00083c624c6e30ee,
        0x5ce35408f2572e46, 0x5b33bae1f8572f1e, 0x230f19d48f68695a, 0x2523f24a77a7a299,
        0x62c645e7b81eb06a, 0x498e30418629ce3b, 0x7d728960a8e96a26, 0x63a0f0c9914aa518,
        0x5aaadd8f776830ed, 0x1a174b99c48f3ab8, 0x4c02960e39eb7658, 0x0c8170c2333b0245,
        0x2285c45725855a64, 0x792224f22fa9c9aa, 0x18d7b44e3911114f, 0x0e6f48172186c423,
        0x6fae3645f97f92d7, 0x34c966893eb725e4, 0x648d0e126e14bf28, 0x3352b00f1f26bd40,
        0x50d3d36c61073f24, 0x705a43828f161a47, 0x5524fca7095b2716, 0x492db54f9262e74b,
        0x7bb3923d0addad5d, 0x6a0c033b89b83984, 0x08e81d751c6b032b, 0x5b819d17c8779f36,
        0x4882c199e9bf22ab, 0x215ae971ee0db3af, 0x7b07de299b358acd, 0x7a90cae9e10c1a29,
        0x55e2f421771c7b89, 0x78ea8938600f0339, 0x1482cafe787cb6c0, 0x75532a9dda67b5f9,
        0x1c5a3210f5917f4e, 0x516f1526fe256186, 0x485b8875f022e839, 0x7ae61bcf69ea790c,
        0x462a36579ff2df26, 0x39dc85665af5b0d0, 0x60ef4077583f795b, 0x7af103c5caa5cd6e,
        0x58ace49e2271864c, 0x3f9ee580088c087e, 0x39b65fbcd598a3f6, 0x1f5601b143edaadf,
        0x194be933723f6989, 0x5914cc79b0907640, 0x16791f26d5a2be16, 0x32df62c26569af82,
        0x43c08d9afc882ba1, 0x420b68623f8c30db, 0x2fbf64fbf4805734, 0x0dff2a817b5fdaf2,
        0x191485d690d5dffb, 0x56fb8274c4fa5f10, 0x67a540dbbac5afb9, 0x6db59582561cc360,
        0x7d9ed82268ab48b5, 0x56fd55504fb81c6f, 0x7165f298456667e4, 0x628b5c2f96647c07
};

static const coeff_t X_ntt[512] = {
        0x45b968be5984376b, 0x49a9c3647853d830, 0x7d13b3eca2d5868c, 0x460cfaa4f29c9d6d,
        0x439d0f4b0455c44b, 0x2f43923ec7d9c7a2, 0x43c6dc615be37195, 0x2bca69e443fb7423,
        0x2dc77e806849f574, 0x52e17521607cb097, 0x6725d1cf311ab0ec, 0x13a76007823422e1,
        0x1fc9a1714b6d191e, 0x66cdd452dc399e20, 0x5575e0ab8a339769, 0x737d99ba21635965,
        0x1e0b2a57a8b41c92, 0x6502539122ec4330, 0x10d220e23c790dd4, 0x2b9ad81c71ede360,
        0x1016c66b9b6a3e7c, 0x0f1c4ae74d5eada4, 0x617d6ca99cda135c, 0x4352a7e72b082a86,
        0x1ed6d621e9e097a4, 0x04c5f604919c8898, 0x13913163b9b9e71b, 0x5ff934a237ae3973,
        0x63776fa29fbcd7be, 0x0d3045fb31146899, 0x06b2c1791c6cdf4d, 0x20ec23b703bc436b,
        0x73fec31c65386f7c, 0x7e0d520a3f7a7b7b, 0x5e72a2d2b0b53323, 0x75215f066320debd,
        0x58f0f48e02a85128, 0x1d73a0f24f5cc742, 0x6fa4fdde8a1a7747, 0x346e4f3d7824be61,
        0x36cfd8f8352669ce, 0x1f084c175c5c3c1b, 0x790498d283f89ca2, 0x0527214c1dbe7af6,
        0x2da6ba619dc35184, 0x7ec0ded61aa0d0b1, 0x2787cd1a36b63140, 0x3357b7070f443d3f,
        0x17403b9f0eb0330b, 0x76a7c0ca70ba6aed, 0x55a0a50a092f778e, 0x52b159c747cc9f70,
        0x7077fe5bd3766018, 0x3bbfdac2d72a6343, 0x79f078ab39197adb, 0x745fbf6e458bacdc,
        0x7f87f5fa47cbecf9, 0x4ed7939015f367d5, 0x454a900024eaaf5e, 0x738d629e275b1d99,
        0x6e364c3754de9ae3, 0x53b2cb1e3673d3c1, 0x4dc6dbcb0c4352af, 0x282b19ed95281763,
        0x30a53bf9230ac522, 0x34ad6e284052611c, 0x042723fad60d3cf1, 0x1f4876c80ea94b4e,
        0x3c657a61ce1fe9a8, 0x7486b13dcb50be82, 0x1eebf2f2770543b0, 0x571a1fa81bcb7d13,
        0x104e298b75032cfe, 0x3d86ef6dd3203c2a, 0x6a5a47f7112d34c3, 0x53bc1532be9619c1,
        0x3fb021859e7a74de, 0x6138f9575e099da8, 0x4084528876d13367, 0x1baa9d7bca474725,
        0x070c9ef08680a407, 0x21e923535bb24687, 0x0418d2e835e1b09c, 0x44c5796480f72f16,
        0x564add544d493c90, 0x48ac2a0c629eefef, 0x0db96a35145a0d9b, 0x35de48fa564c1003,
        0x45473bd6fc580137, 0x68f36c5e30174ca7, 0x26ea7979764757bd, 0x1b2e786afdc8bbbb,
        0x29b9d1eb7fde48df, 0x44793e310adaa8eb, 0x3608ea801c93a582, 0x6c9cfd7b64a58ea3,
        0x3adb76d92bed82b5, 0x7cf50c79a989dc64, 0x27b020be34911946, 0x05aabd4ee29008c5,
        0x35c73ce7754d9feb, 0x6acd2f0096a03b08, 0x60bdc32d5ad60f24, 0x43f029f0401cf61f,
        0x652777c2fbd5c19c, 0x5570556621517644, 0x26c0076d7f9526d3, 0x2f5ffd9fa17a702d,
        0x26a572bb590fb709, 0x51557f5afef63832, 0x47d85386fbcc1386, 0x046a140f9f2c2f60,
        0x02ad1305552a7e64, 0x38ca6c6bc037e305, 0x130da688b29af931, 0x6fe4ee110eb49523,
        0x4c4817afa40a616f, 0x1f15c7990a52f60b, 0x3b4f607a9c649df4, 0x4e110c41c6cf6fe8,
        0x24a5b33eb8e6aac2, 0x2afb0a8b3bec604c, 0x5d074de142a25dc4, 0x3f904214158fb95c,
        0x10b0c0dd9eb9cb1a, 0x75d2aa34c836bff3, 0x7614d14f601f2e68, 0x356e8c84905a1e16,
        0x3c3618a2ac522599, 0x3e35365a921a8774, 0x0007c5f0d0538186, 0x43b79c7cc155d8d9,
        0x13094a8d0bea224a, 0x3fb68eafd34027d4, 0x2d4ea0302d0707d8, 0x3ef481c832f5e5d4,
        0x05ddb7364dd5abc6, 0x57eced267dfe8fba, 0x78ebdd7a1746b2b6, 0x081ab7e1d8dc9b70,
        0x3fd37a2be3cbd5bf, 0x72c4e8417b5b871f, 0x2fb6bc8eb00510a6, 0x7c65d841b48e27fa,
        0x2be60a0b1e23593e, 0x507ee0ad053bfa20, 0x4e1db5650db0a9e2, 0x4a29dc0c8e349c60,
        0x471425c89f08deaa, 0x28b578f2ccbd1448, 0x00ead836760656b7, 0x4ea1909e1d8fa3f4,
        0x4aaf0e4a4ad7ac76, 0x5e4579cab9bf37d3, 0x535b443498e92c36, 0x4398a341a06f5c3e,
        0x5e3a79468778e2d7, 0x3bc07bfeb9211a88, 0x2214d5b5e30fd2cd, 0x182c23768878a4a2,
        0x2de6c97679ffcd32, 0x7b86d3e12cf82d6d, 0x7b95c3393bf23fdf, 0x2e9bf09d835b760d,
        0x05018e10195591c8, 0x6396b1166e18a561, 0x531a61fcd7b22803, 0x7dff20f4e34883f6,
        0x5c36432c9cca474e, 0x1447d6ef49bf9840, 0x7dc66d2977b66cef, 0x3197a35afbb1dddb,
        0x13f4af39e45ce948, 0x3a0c2d9a6b030474, 0x60fcd097a3c24397, 0x7365ba125d08976d,
        0x6975647a12e06c19, 0x461acf702ccffc14, 0x636aca1ba35dfc54, 0x6035f7023be55b89,
        0x1e7803e4001ce29d, 0x6bebd170ff45fe81, 0x499838cd46a0e907, 0x49d48232e6938134,
        0x49d219c28550837c, 0x1f4c7c0a1e883fc2, 0x58cc3f40bc9795c1, 0x73dc9d9df3c24b78,
        0x7cf7a1d9b101ebfd, 0x577404bbc838ea47, 0x6d9ffaac837efe60, 0x25b55f07facfa799,
        0x40d3ff432d22ff7a, 0x2c6ac5f7a2a41210, 0x2aca9c32a4c1d157, 0x4db640bf430db140,
        0x3cb4437a515f897b, 0x2203036d4ae805d8, 0x7e0c38ae2b6707b5, 0x5512289c8891eed2,
        0x558df5dcdb7206fb, 0x6dfee6a23b7ad90d, 0x25c8ad6410b7bfc8, 0x19a83c1a6079e44b,
        0x00462926e7c90c45, 0x13e02d82e1667c52, 0x2613d6ced9dc7285, 0x2b947812d71a90ef,
        0x5612aaef6c201b90, 0x5d3f488418cd78e0, 0x5def7ff8f7f4adce, 0x75b831148c7a36af,
        0x74f72cf2b2d03452, 0x28bd8ef19c2ab9ab, 0x340531d6edc5c75a, 0x6a6fa24256ef4bb0,
        0x3b6ecda72d5df4e4, 0x0904575b052f0672, 0x3fe25a7fb675be89, 0x0dd228f9127ede40,
        0x6710035133f201bb, 0x4c6bfa1db92c5b79, 0x20be1d334b0f8f5b, 0x51f3ddcd4d4084a4,
        0x215f471598c9721d, 0x55a27c92fdbf8450, 0x73f87f8e5e3b69ee, 0x3e0b9e968277e183,
        0x6a4b16976c7f854c, 0x78905f46bf327484, 0x32131f125188aadd, 0x395d59e08437c8a4,
        0x55177e0a5836c7ce, 0x21e6247a19fd5cce, 0x4d3a2c0a2e43348f, 0x05bf2cbfc39738a3,
        0x77684f2f735deb2f, 0x205dcf84b315f35b, 0x4ee2dd745762871d, 0x732802feabaae1c5,
        0x283024c792f57931, 0x7fc55aff9a3a21ec, 0x213c32712fe8c980, 0x2fb62889cd4dfdf3,
        0x40cfaa92605cf31b, 0x7f9a909858d98ef6, 0x43772f291e30b7e6, 0x1cbd9bf257dbe690,
        0x5d3b70f2593e51a8, 0x138cf1f671337dc5, 0x6999344aced315f0, 0x4d6f9cd1a585cac0,
        0x259952c2f68b054d, 0x0f0cd758ab224a49, 0x7d643aa2a4907358, 0x21c11e4c5ebd7332,
        0x2f694f9b26a5d3e1, 0x4580780764cd0788, 0x53503fc802fdd5b5, 0x6a42d6a9557abed6,
        0x494ac3c6832bed5e, 0x51d636d9e3d0749b, 0x4f7a23788ea49245, 0x5f27128eabc4b554,
        0x30535483bbf3d701, 0x530f013566983c5d, 0x42a7a64ede2c3a23, 0x1578d7633d02ad86,
        0x4de150485ce310b5, 0x17184fd72cf337a2, 0x7e7e032e870ca9b7, 0x466cf7fd6d81f4ed,
        0x0f2ae329ac62340a, 0x64c2edc8f157b280, 0x62d050168b591950, 0x5b2c0fb7e3ddbc1b,
        0x346824325d11bae6, 0x5fadc2468d2bcd52, 0x177b3b4f460bcb4f, 0x1157cd3f4c0d2bf3,
        0x578e4edfc43de011, 0x0916fcb4a5f57389, 0x3ebd3b37916480f3, 0x7361cb172cb34a9b,
        0x2cd3f4c128df5186, 0x111e7acecd3510f7, 0x3e793477b5e3bcc6, 0x75ad35de07cddfff,
        0x72c7a5f86dfe0098, 0x130b8c0d3d2b735d, 0x2d626b841867dfa5, 0x789f184b2347d58d,
        0x2178c046311d2185, 0x7d15ba4a86a545df, 0x71951be9c1dea375, 0x60dd212e6b7d0278,
        0x6602a5f4b7fccca9, 0x65efc7ab9fc40c9c, 0x4c89ebf2100ba4fc, 0x525d9dcc9dd17225,
        0x1043890e01364a7c, 0x2c21a74bdb914795, 0x4813b08329d2f060, 0x556ce520fe5cbc96,
        0x10ff7e1375c1fe09, 0x25ceae595f0484cf, 0x0173ea2e4b369ede, 0x7fc96572ae271e2f,
        0x5c5b5ea9bddf1276, 0x77188b13566486a0, 0x2921cfbd0e032a7c, 0x227623ad95c688fb,
        0x724e872c408528e8, 0x673c0fa4a86a7d8e, 0x0bc27caf266f0cd1, 0x492bdacaf02b2f9c,
        0x7e668fec0af365c2, 0x49baa6d77ca5eba5, 0x0f0a91e3afeebd48, 0x4c811818c1c79c26,
        0x086900fb7d75156f, 0x180f6ec482432427, 0x2e30a0d71e027821, 0x6685d645b7bb3f88,
        0x468eac51b9af107e, 0x7e0ebf574c5d9055, 0x4868e72ba631829f, 0x6d4588e342253994,
        0x0de65eaa6582d3b2, 0x0c2140ba30158809, 0x4e26ca155a465a3c, 0x762c018ac65dab4b,
        0x282cbe3db3eb1840, 0x49d15630d10a9a78, 0x19bca486c58dacc6, 0x309e01ad4c96ac30,
        0x164784f25a28e277, 0x3c95055de24bb6ff, 0x1604362889696eac, 0x64cc6711e1a60a69,
        0x63fc11bc3117fb40, 0x2b6055c639643243, 0x368a8f9127de498f, 0x6fc7af12d4b05320,
        0x7f127a15d0cd51cc, 0x1a1d875c9c47315c, 0x4c328d7d3143cc5d, 0x6fe0f0f7968e5454,
        0x429175cea59abf30, 0x5727ca7717d3cf42, 0x72eefb393c31d1c3, 0x6d5ad71f8b4f6ae6,
        0x449e8935faabe23d, 0x726ceabd8e307945, 0x5fd5e0983e5df3ee, 0x510f9bcaf4a55c17,
        0x2995e077d2ff8541, 0x1d57c6b1024de44c, 0x6282302069b0ad71, 0x659ee093812f49db,
        0x007b271db7f748f5, 0x3b251c3230429f4b, 0x1852c009a813f359, 0x3f03344dffce9bd5,
        0x10d208adf7d9c2b0, 0x39f5b269826c02a2, 0x043cd94eb3d9062e, 0x66a61d75dad93650,
        0x361b78ecd7ec7f48, 0x2b4d77fbea5909ce, 0x1a6a30f98c340ab7, 0x27903aed6b914bbc,
        0x29836dbdc86e6e99, 0x17b4c7764ebd59e6, 0x32d93532eebf4d7f, 0x0f4116d209e53461,
        0x37fd2a01a307ec34, 0x79b6424f89c008a6, 0x5e618d0ee4e36641, 0x6c9cbdfa9e1daa2a,
        0x5bd67ebd44f4c67e, 0x47d079e77cf73b5b, 0x68dfbe129c11a4b2, 0x54a6e8eae400ef34,
        0x299e8ae53e089f2d, 0x5636c283e4ff6926, 0x5b270ff0120d30b8, 0x46735da4835f7259,
        0x52650c825eaf87d2, 0x7a33f8c5129e51d6, 0x4fb23068541db13c, 0x7a4bdd4a85e346db,
        0x288ed44cd0ca0b22, 0x0b6c2cc50eee3e70, 0x5c023bc72d35daf5, 0x45e40a07be9a1b8d,
        0x55d0a40034012bff, 0x4ddec258c886de04, 0x4b964dd4d814177a, 0x7d79204ed3330cab,
        0x723fdb0103c76579, 0x5c1310e24a557105, 0x3ea0444a66002b4a, 0x6c7b9a1c050a8e92,
        0x3489d08993a72f66, 0x7441f991cd8dafce, 0x15964ab956b8eaca, 0x7fe9dc7b0ac45047,
        0x6f46da9c873f6f49, 0x0a62c60df253bb66, 0x564b2ce73fde1cee, 0x085f6baa0b1eba67,
        0x0d6514169bbd3b7a, 0x71512c27bc0f5a57, 0x4e8520f1f43c34ba, 0x63f641598e1d9b66,
        0x621ce232b5a985f8, 0x5c21422c76082035, 0x6520f625eeaf4f75, 0x6d7df33d6ff7497e,
        0x6097ede97c2b818f, 0x7f574a458cef3e11, 0x439bb92b2497b0d7, 0x103ab35e982d65f3,
        0x02e75a3505bddfdd, 0x515fdbd2397fcdf0, 0x21a49b72aa27b03b, 0x2d576ae7b83420a2,
        0x2533080317ee32be, 0x166f578cdbd3bc15, 0x1e6fc5e1f218a039, 0x266913ebad645463,
        0x54d610eb81e7b622, 0x10aa362aa3799fdf, 0x3fec946f8dce0d2e, 0x1ef1422b539bd53d,
        0x063071e6d3c48f5b, 0x4cb7a840c59d8446, 0x2811359372cfee19, 0x37d351ed5098be72,
        0x1e4d1927f5f35237, 0x6dc75c5d5dadbafd, 0x2efed3b33259d894, 0x42adcef08ad54b27,
        0x028a03eaee0d8a83, 0x50916ae6273e1c28, 0x31bcc8340c87db4d, 0x7b2d1ac73d425fcc,
        0x26c9bc544791ae68, 0x242b9846b0685bb5, 0x68e1f1ec5dfb4743, 0x4427df183015bef3,
        0x1ef96aae6bd1b7d9, 0x259eb654d6901544, 0x218a4b3724437190, 0x34be921cfff4bea7,
        0x01ae9cf112fdad71, 0x49d8dec3032a8ecd, 0x176d04b19617faca, 0x1f603cdc038eebcd,
        0x4c66619b77531cbd, 0x0a1b1f487300d93d, 0x1d9171962c3ffd09, 0x4334ee13c3171d9a,
        0x283fd6a8a434944f, 0x2827c178c6d9715f, 0x3faa759c6862509b, 0x1ea6902295b5eb8b,
        0x53ca8a035aade0b2, 0x2e511822a8afd581, 0x7eea0ba88867f1e9, 0x0629819bcc09bb23,
        0x40b1cdea7d5595f7, 0x29f343308da5a608, 0x1864457380b920d9, 0x4a27bf154548239a,
        0x4b3f18576353fef3, 0x63523bdf6819b68d, 0x558f95489c74ca01, 0x2f935709bca444cf,
        0x6543d26fba132910, 0x44c1be7de91c1784, 0x5e5d1ce03cf45bde, 0x102e338b9791a982,
        0x51e73b8355a3565d, 0x05ce4f8a5e3717a6, 0x68eb4f97f83b2e93, 0x0470705932a32561,
        0x01f817bb34c4d2f1, 0x011f862ab64b3a32, 0x2520bc7a115f0a8d, 0x20874524dc755471,
        0x21dda90efbe2df3c, 0x04d04dd94ea92302, 0x133f58bb3591039c, 0x5fbad975b62b29f2,
        0x15d5e4e0c7c33081, 0x144c5fb96c473adc, 0x33afd7584206c6e7, 0x31cfbcf393288c42,
        0x7ce7f34f1665106d, 0x668624020d19b91a, 0x6e264275b07419b7, 0x122572e39b1ce32f,
        0x1c6f681306ae90b3, 0x66b3197bb8c76448, 0x6fca7675554aadbe, 0x7a95536f958ff6f4,
        0x736d801373fd277f, 0x569a6d1f05a93660, 0x153ac3ee01663c2d, 0x64dfc40b0f315321
};

void accelc_TEST_NTT() {
    coeff_t _X_ntt[sizeof(X) / sizeof(coeff_t)];
    coeff_t _X[sizeof(X) / sizeof(coeff_t)];

    uint64_t t1 = __rdtsc();
    accelc_NTT(X, sizeof(X) / sizeof(coeff_t), _X_ntt, PrimitiveRoot, Prime);
    uint64_t t2 = __rdtsc();
    printf("accelc_NTT <512> time: %lld, result: %d\n", t2 - t1, memcmp(_X_ntt, X_ntt, sizeof(X_ntt)));

    t1 = __rdtsc();
    accelc_iNTT(_X_ntt, sizeof(X) / sizeof(coeff_t), _X, PrimitiveRoot, Prime);
    t2 = __rdtsc();
    printf("accelc_iNTT <512> time: %lld, result: %d\n", t2 - t1, memcmp(_X, X, sizeof(X)));

    memset(_X_ntt, 0, sizeof(_X_ntt));
    memset(_X, 0, sizeof(_X));

    t1 = __rdtsc();
    accelc_FNTT_base2(X, sizeof(X) / sizeof(coeff_t), _X_ntt, PrimitiveRoot, Prime);
    t2 = __rdtsc();
    printf("accelc_FNTT_base2 <512> time: %lld, result: %d\n", t2 - t1, memcmp(_X_ntt, X_ntt, sizeof(X_ntt)));

    t1 = __rdtsc();
    accelc_iFNTT_base2(_X_ntt, sizeof(X) / sizeof(coeff_t), _X, PrimitiveRoot, Prime);
    t2 = __rdtsc();
    printf("accelc_iFNTT_base2 <512> time: %lld, result: %d\n", t2 - t1, memcmp(_X, X, sizeof(X)));
}