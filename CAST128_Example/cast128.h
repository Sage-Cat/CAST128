#ifndef CAST128_H
#define CAST128_H

#include <Qt>

class CAST128 {
public:
    enum {
        KEY_LEN = 128 / 32,
        MSG_LEN = 2
    };

    typedef quint32 Key[ KEY_LEN ];
    typedef quint32 Message[ MSG_LEN ];

public:
    CAST128();

    void encrypt( const Key key, Message msg );
    void decrypt( const Key key, Message msg );

private:
    void run( const Key key, Message msg, bool reverse = false );

private:
    typedef quint32 SType[ 256 ];

    static const SType S1;
    static const SType S2;
    static const SType S3;
    static const SType S4;
    static const SType S5;
    static const SType S6;
    static const SType S7;
    static const SType S8;
};

#endif // CAST128_H
