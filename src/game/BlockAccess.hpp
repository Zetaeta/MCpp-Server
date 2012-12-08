
#ifndef BLOCKACCESS_HPP
#define BLOCKACCESS_HPP

namespace MCServer {

class Block;

/**
 * operator[]-based access to blocks in a chunk.
 */
template <int N>
class BlockAccess;

/**
 * Due to difficulties in forcing const-correctness in non-reference objects
 * that act as references to something else being passed around,
 * a separate class must be made to represent a const BlockAccess.
 */
template <int N>
class ConstBlockAccess;

template <>
class BlockAccess<1> {
public:
    BlockAccess(Chunk &);

    BlockAccess<2> operator[](int);
    ConstBlockAccess<2> operator[](int) const;

    operator ConstBlockAccess<1>() const;

private:
    Chunk &chunk;
};

template <>
class BlockAccess<2> {
public:
    BlockAccess(Chunk &, int x);

    BlockAccess<3> operator[](int);
    ConstBlockAccess<3> operator[](int) const;

    operator ConstBlockAccess<2>() const;

private:
    Chunk &chunk;
    int x;
};

template <>
class BlockAccess<3> {
public:
    BlockAccess(Chunk &, int x, int z);

    Block operator[](int);
    const Block operator[](int) const;

    operator ConstBlockAccess<3>() const;

private:
    Chunk &chunk;
    int x;
    int z;
};

template <>
class ConstBlockAccess<1> {
public:
    ConstBlockAccess(const Chunk &);
    ConstBlockAccess<2> operator[](int) const;

private:
    const Chunk &chunk;
};

template <>
class ConstBlockAccess<2> {
public:
    ConstBlockAccess(const Chunk &, int x);

    ConstBlockAccess<3> operator[](int) const;

private:
    const Chunk &chunk;
    int x;
};

template <>
class ConstBlockAccess<3> {
public:
    ConstBlockAccess(const Chunk &, int x, int z);

    const Block operator[](int) const;

private:
    const Chunk &chunk;
    int x;
    int z;
};

}

#endif

