#include <cstring>
#include "encryption.h"

using namespace ral;

alg crypto = noone;
keytype key;
int count;

std::ostream& horrible_manip(std::ostream& os, alg n, keytype& _key)
{
    crypto = n;
    key = _key;
    count = 0;
    return os;
}

namespace ral
{
    my_manip encrypt(alg n, keytype& key)
    {
        return my_manip(horrible_manip, n, key);
    }

    my_manip encrypt(alg n, std::vector<unsigned char>& key)
    {
        keytype g;
        g.push_back(key);
        return my_manip(horrible_manip, n, g);
    }

    my_manip encrypt(alg n)
    {
        keytype g;
        return my_manip(horrible_manip, n, g);
    }
}

ourbuf::int_type ourbuf::overflow(int_type c)
{
    if (c != EOF)
    {
        switch (crypto)
        {
            case polisub:
            {
                auto alphc = static_cast<int>(key.size());
                int_type dst = c;
                dst = (key[count][c >> 4] << 4) & 0xF0;
                ++count;
                if (count == alphc)
                    count = 0;
                dst |= key[count][c & 0xF];
                ++count;
                if (count == alphc)
                    count = 0;
                c = dst;

            } break;
        }
    }
    putchar(c);
    return c;
}
namespace ral
{
    ourbuf ob;
    std::ostream cout(&ob);
}