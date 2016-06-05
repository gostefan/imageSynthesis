/*!\file RGBE.cpp
   \author Wojciech Jarosz
 */

#include <Math/RGBE.h>

namespace Math
{

//
// Generated using:
// 
// {
//     for (int i = 0; i < 256; ++i)
//     {
//         printf("%.8ef, ", ldexp(1.0f, i-(int)(128+8)));
//         if ((i+1) % 4 == 0)
//             printf("\n");
//     }
// }
//
const float RGBE::s_ldexp[256] =
{
    0.00000000e+00f, 2.29588740e-41f, 4.59177481e-41f, 9.18354962e-41f,
    1.83670992e-40f, 3.67341985e-40f, 7.34683969e-40f, 1.46936794e-39f,
    2.93873588e-39f, 5.87747175e-39f, 1.17549435e-38f, 2.35098870e-38f,
    4.70197740e-38f, 9.40395481e-38f, 1.88079096e-37f, 3.76158192e-37f,
    7.52316385e-37f, 1.50463277e-36f, 3.00926554e-36f, 6.01853108e-36f,
    1.20370622e-35f, 2.40741243e-35f, 4.81482486e-35f, 9.62964972e-35f,
    1.92592994e-34f, 3.85185989e-34f, 7.70371978e-34f, 1.54074396e-33f,
    3.08148791e-33f, 6.16297582e-33f, 1.23259516e-32f, 2.46519033e-32f,
    4.93038066e-32f, 9.86076132e-32f, 1.97215226e-31f, 3.94430453e-31f,
    7.88860905e-31f, 1.57772181e-30f, 3.15544362e-30f, 6.31088724e-30f,
    1.26217745e-29f, 2.52435490e-29f, 5.04870979e-29f, 1.00974196e-28f,
    2.01948392e-28f, 4.03896783e-28f, 8.07793567e-28f, 1.61558713e-27f,
    3.23117427e-27f, 6.46234854e-27f, 1.29246971e-26f, 2.58493941e-26f,
    5.16987883e-26f, 1.03397577e-25f, 2.06795153e-25f, 4.13590306e-25f,
    8.27180613e-25f, 1.65436123e-24f, 3.30872245e-24f, 6.61744490e-24f,
    1.32348898e-23f, 2.64697796e-23f, 5.29395592e-23f, 1.05879118e-22f,
    2.11758237e-22f, 4.23516474e-22f, 8.47032947e-22f, 1.69406589e-21f,
    3.38813179e-21f, 6.77626358e-21f, 1.35525272e-20f, 2.71050543e-20f,
    5.42101086e-20f, 1.08420217e-19f, 2.16840434e-19f, 4.33680869e-19f,
    8.67361738e-19f, 1.73472348e-18f, 3.46944695e-18f, 6.93889390e-18f,
    1.38777878e-17f, 2.77555756e-17f, 5.55111512e-17f, 1.11022302e-16f,
    2.22044605e-16f, 4.44089210e-16f, 8.88178420e-16f, 1.77635684e-15f,
    3.55271368e-15f, 7.10542736e-15f, 1.42108547e-14f, 2.84217094e-14f,
    5.68434189e-14f, 1.13686838e-13f, 2.27373675e-13f, 4.54747351e-13f,
    9.09494702e-13f, 1.81898940e-12f, 3.63797881e-12f, 7.27595761e-12f,
    1.45519152e-11f, 2.91038305e-11f, 5.82076609e-11f, 1.16415322e-10f,
    2.32830644e-10f, 4.65661287e-10f, 9.31322575e-10f, 1.86264515e-09f,
    3.72529030e-09f, 7.45058060e-09f, 1.49011612e-08f, 2.98023224e-08f,
    5.96046448e-08f, 1.19209290e-07f, 2.38418579e-07f, 4.76837158e-07f,
    9.53674316e-07f, 1.90734863e-06f, 3.81469727e-06f, 7.62939453e-06f,
    1.52587891e-05f, 3.05175781e-05f, 6.10351562e-05f, 1.22070312e-04f,
    2.44140625e-04f, 4.88281250e-04f, 9.76562500e-04f, 1.95312500e-03f,
    3.90625000e-03f, 7.81250000e-03f, 1.56250000e-02f, 3.12500000e-02f,
    6.25000000e-02f, 1.25000000e-01f, 2.50000000e-01f, 5.00000000e-01f,
    1.00000000e+00f, 2.00000000e+00f, 4.00000000e+00f, 8.00000000e+00f,
    1.60000000e+01f, 3.20000000e+01f, 6.40000000e+01f, 1.28000000e+02f,
    2.56000000e+02f, 5.12000000e+02f, 1.02400000e+03f, 2.04800000e+03f,
    4.09600000e+03f, 8.19200000e+03f, 1.63840000e+04f, 3.27680000e+04f,
    6.55360000e+04f, 1.31072000e+05f, 2.62144000e+05f, 5.24288000e+05f,
    1.04857600e+06f, 2.09715200e+06f, 4.19430400e+06f, 8.38860800e+06f,
    1.67772160e+07f, 3.35544320e+07f, 6.71088640e+07f, 1.34217728e+08f,
    2.68435456e+08f, 5.36870912e+08f, 1.07374182e+09f, 2.14748365e+09f,
    4.29496730e+09f, 8.58993459e+09f, 1.71798692e+10f, 3.43597384e+10f,
    6.87194767e+10f, 1.37438953e+11f, 2.74877907e+11f, 5.49755814e+11f,
    1.09951163e+12f, 2.19902326e+12f, 4.39804651e+12f, 8.79609302e+12f,
    1.75921860e+13f, 3.51843721e+13f, 7.03687442e+13f, 1.40737488e+14f,
    2.81474977e+14f, 5.62949953e+14f, 1.12589991e+15f, 2.25179981e+15f,
    4.50359963e+15f, 9.00719925e+15f, 1.80143985e+16f, 3.60287970e+16f,
    7.20575940e+16f, 1.44115188e+17f, 2.88230376e+17f, 5.76460752e+17f,
    1.15292150e+18f, 2.30584301e+18f, 4.61168602e+18f, 9.22337204e+18f,
    1.84467441e+19f, 3.68934881e+19f, 7.37869763e+19f, 1.47573953e+20f,
    2.95147905e+20f, 5.90295810e+20f, 1.18059162e+21f, 2.36118324e+21f,
    4.72236648e+21f, 9.44473297e+21f, 1.88894659e+22f, 3.77789319e+22f,
    7.55578637e+22f, 1.51115727e+23f, 3.02231455e+23f, 6.04462910e+23f,
    1.20892582e+24f, 2.41785164e+24f, 4.83570328e+24f, 9.67140656e+24f,
    1.93428131e+25f, 3.86856262e+25f, 7.73712525e+25f, 1.54742505e+26f,
    3.09485010e+26f, 6.18970020e+26f, 1.23794004e+27f, 2.47588008e+27f,
    4.95176016e+27f, 9.90352031e+27f, 1.98070406e+28f, 3.96140813e+28f,
    7.92281625e+28f, 1.58456325e+29f, 3.16912650e+29f, 6.33825300e+29f,
    1.26765060e+30f, 2.53530120e+30f, 5.07060240e+30f, 1.01412048e+31f,
    2.02824096e+31f, 4.05648192e+31f, 8.11296384e+31f, 1.62259277e+32f,
    3.24518554e+32f, 6.49037107e+32f, 1.29807421e+33f, 2.59614843e+33f,
    5.19229686e+33f, 1.03845937e+34f, 2.07691874e+34f, 4.15383749e+34f,
    8.30767497e+34f, 1.66153499e+35f, 3.32306999e+35f, 6.64613998e+35f
};

} // namespace Math
