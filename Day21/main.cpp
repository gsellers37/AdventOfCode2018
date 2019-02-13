#include <iostream>
#include <set>

int main(int argc, char const *argv[])
{
    long rx[6] = {0};
    int ipr = 1;

    long iters = 0;
    
    std::set<long> repeats;
    long lastSeen;
    bool partOneSolved = false;
    while (true){
        iters ++;
        switch (rx[ipr]){
            case 0: rx[4] = 123; break;
            case 1: rx[4] = rx[4] & 456; break;
            case 2: rx[4] = rx[4] == 72 ? 1 : 0; break;
            case 3: rx[1] = rx[4] + rx[1]; break;
            case 4: rx[1] = 0; break;
            case 5: rx[4] = 0; break;
            case 6: rx[3] = rx[4] | 65536; break;
            case 7: rx[4] = 10552971; break;
            case 8: rx[5] = rx[3] & 255; break;
            case 9: rx[4] = rx[5] + rx[4]; break;
            case 10: rx[4] = rx[4] & 16777215; break;
            case 11: rx[4] = rx[4] * 65899; break;
            case 12: rx[4] = rx[4] & 16777215; break;
            case 13: rx[5] = 256 > rx[3] ? 1 : 0; break;
            case 14: rx[1] = rx[5] + rx[1]; break;
            case 15: rx[1] = rx[1] + 1; break;
            case 16: rx[1] = 27; break;
            case 17: rx[5] = 0; break;
            case 18: rx[2] = rx[5] + 1; break;
            case 19: rx[2] = rx[2] * 256; break;
            case 20: rx[2] = rx[2] > rx[3] ? 1 : 0; break;
            case 21: rx[1] = rx[2] + rx[1]; break;
            case 22: rx[1] = rx[1] + 1; break;
            case 23: rx[1] = 25; break;
            case 24: rx[5] = rx[5] + 1; break;
            case 25: rx[1] = 17; break;
            case 26: rx[3] = rx[5]; break;
            case 27: rx[1] = 7; break;
            case 28: rx[5] = rx[4] == rx[0] ? 1 : 0; break;
            case 29: rx[1] = rx[5] + rx[1]; break;
            case 30: rx[1] = 5; break;

            default:
                return iters;
        }
        if (rx[ipr] == 28)
        {
            // Part 2
            if (repeats.count(rx[4]) > 0)
            {
                printf("Part 2: %lu\n", lastSeen);
                return 0;
            }
            repeats.insert(rx[4]);
            lastSeen = rx[4];

            if(!partOneSolved)
            {
                printf("Part 1: %lu\n", rx[4]);
                partOneSolved = true;
            }

        }
        rx[ipr]++;
        // printf("%lu: %lu\n",iters,rx[ipr]);
    }
    return 0;
}
