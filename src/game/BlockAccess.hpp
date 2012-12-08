
#ifndef BLOCKACCESS_HPP
#define BLOCKACCESS_HPP

namespace MCServer {

/**
 * operator[]-based access to blocks in a chunk.
 */
template <int N>
class BlockAccess;

template <>
class BlockAccess<1> {
public:
    BlockAccess(Chunk &, int x);
    BlockAccess<2> operator[](int);
    const BlockAccess<2> operator[](int) const;

    operator const BlockAccess<1>() const;
    operator BlockAccess<1>();
private:
    BlockAccess(const BlockAccess &);
    Chunk &chunk;
    int xPos;
};

template <>
class BlockAccess<2> {
public:
    BlockAccess(Chunk &, int x, int y);

    BlockAccess<3> operator[](int);
    const BlockAccess<3> operator[](int) const;

    operator const BlockAccess<2>() const;
    operator BlockAccess<2>();
private:
    BlockAccess(const BlockAccess &);
    Chunk &chunk;
    int xPos;
    int zPos;
};

template <>
class BlockAccess<3> {
public:
    BlockAccess(Chunk &, int x, int y, int z);

    Block operator[](int);
    const Block operator[](int) const;

    operator const BlockAccess<3>() const;
    operator BlockAccess<3>();
private:
    BlockAccess(const BlockAccess &);
    Chunk &chunk;
    int xPos;
    int zPos;
    int yPos;
};

}

#endif

