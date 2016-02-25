#ifndef _PASSWORD_HASHER_COMMON_HPP
#define _PASSWORD_HASHER_COMMON_HPP

#include <functional>

/// For now PasswordHasher is just a typedef, but later, if we can, we will
/// replace it by a true cryptographic hasher.
typedef std::hash<std::string> PasswordHasher;

#endif// _PASSWORD_HASHER_COMMON_HPP
