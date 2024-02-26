#include "defines.hpp"

#include "errors.hpp"

namespace ylang {

  bool operator<(const address_t& lhs, const address_t& rhs) {
    return lhs.address < rhs.address;
  }

  bool operator>(const address_t& lhs, const address_t& rhs) {
    return lhs.address > rhs.address;
  }

  bool operator==(const address_t& lhs, const address_t& rhs) {
    return lhs.address == rhs.address;
  }

  bool operator!=(const address_t& lhs, const address_t& rhs) {
    return lhs.address != rhs.address;
  }

  bool operator<=(const address_t& lhs, const address_t& rhs) {
    return lhs.address <= rhs.address;
  }

  bool operator>=(const address_t& lhs, const address_t& rhs) {
    return lhs.address >= rhs.address;
  }
  
  bool operator<(const address_t& lhs, const int64_t& rhs) {
    return lhs.address < rhs;
  }

  bool operator>(const address_t& lhs, const int64_t& rhs) {
    return lhs.address > rhs;  
  }

  bool operator==(const address_t& lhs, const int64_t& rhs) {
    return lhs.address == rhs;
  }

  bool operator<(const address_t& lhs, const uint64_t& rhs) {
    return lhs.address < rhs;    
  }

  bool operator==(const address_t& lhs, const uint64_t& rhs) {
    return lhs.address == rhs;
  }
  
  bool operator>(const address_t& lhs, const uint64_t& rhs) {
    return lhs.address > rhs;
  }
  
  bool operator<(const address_t& lhs, const float& rhs) {
    return lhs.address < rhs;
  }
  
  bool operator==(const address_t& lhs, const float& rhs) {
    return std::abs(lhs.address - rhs) < std::numeric_limits<float>::epsilon();
  }

  bool operator>(const address_t& lhs, const float& rhs) {
    return lhs.address > rhs;
  }

  bool operator<(const address_t& lhs, const double& rhs) {
    return lhs.address < rhs;
  }

  bool operator>(const address_t& lhs, const double& rhs) {
    return lhs.address > rhs;
  }

  bool operator==(const address_t& lhs, const double& rhs) {
    return std::abs(lhs.address - rhs) < std::numeric_limits<double>::epsilon();
  }
  
  bool operator<(const address_t& lhs, const RegisterType& rhs) {
    return lhs.address < static_cast<uint64_t>(rhs);
  }

  bool operator>(const address_t& lhs, const RegisterType& rhs) {
    return lhs.address > static_cast<uint64_t>(rhs);
  }

  bool operator==(const address_t& lhs, const RegisterType& rhs) {
    return lhs.address == static_cast<uint64_t>(rhs);
  }
  
  address_t operator+(const address_t& lhs, const address_t& rhs) {
    return address_t{lhs.address + rhs.address};
  }

  address_t operator-(const address_t& lhs, const address_t& rhs) {
    if (lhs.address < rhs.address) {
      throw RuntimeError("Address underflow");
    }
    return address_t{lhs.address - rhs.address};
  }

  address_t operator+(const address_t& lhs, const int64_t& rhs) {
    return address_t{lhs.address + rhs};
  }

  address_t operator-(const address_t& lhs, const int64_t& rhs) {
    if (lhs.address < rhs) {
      throw RuntimeError("Address underflow");
    }
    return address_t{lhs.address - rhs};
  }

  address_t operator+(const address_t& lhs, const uint64_t& rhs) {
    return address_t{lhs.address + rhs};
  }

  address_t operator-(const address_t& lhs, const uint64_t& rhs) {
    if (lhs.address < rhs) {
      throw RuntimeError("Address underflow");
    }
    return address_t{lhs.address - rhs};
  }

  address_t operator+(const address_t& lhs, const RegisterType& rhs) {
    return address_t{lhs.address + static_cast<uint64_t>(rhs)};
  }

  address_t operator-(const address_t& lhs, const RegisterType& rhs) {
    if (lhs.address < static_cast<uint64_t>(rhs)) {
      throw RuntimeError("Address underflow");
    }
    return address_t{lhs.address - static_cast<uint64_t>(rhs)};
  }

  address_t operator+(const int64_t& lhs, const address_t& rhs) {
    return address_t{lhs + rhs.address};
  }

  address_t operator-(const int64_t& lhs, const address_t& rhs) {
    if (lhs < rhs.address) {
      throw RuntimeError("Address underflow");
    }
    return address_t{lhs - rhs.address};
  }
  
  address_t operator+(const uint64_t& lhs, const address_t& rhs) {
    return address_t{lhs + rhs.address};
  }

  address_t operator-(const uint64_t& lhs, const address_t& rhs) {
    if (lhs < rhs.address) {
      throw RuntimeError("Address underflow");
    }
    return address_t{lhs - rhs.address};
  }

  address_t operator+(const RegisterType& lhs, const address_t& rhs) {
    return address_t{static_cast<uint64_t>(lhs) + rhs.address};
  }

  address_t operator-(const RegisterType& lhs, const address_t& rhs) {
    if (static_cast<uint64_t>(lhs) < rhs.address) {
      throw RuntimeError("Address underflow");
    }
    return address_t{static_cast<uint64_t>(lhs) - rhs.address};
  }

} // namespace ylang
