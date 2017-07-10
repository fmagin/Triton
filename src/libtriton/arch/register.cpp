//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <triton/exceptions.hpp>
#include <triton/register.hpp>
#include <triton/cpuInterface.hpp>



namespace triton {
  namespace arch {

    ////////////////////////////////////////////
    // RegisterSpec
    ////////////////////////////////////////////

    RegisterSpec::RegisterSpec(triton::arch::registers_e regId, std::string name, triton::arch::registers_e parent, triton::uint32 high, triton::uint32 low)
      : BitsVector(high, low),
        name(name),
        id(regId),
        parent(parent) {
    }


    RegisterSpec::RegisterSpec(const RegisterSpec& other)
      : BitsVector(other) {
        this->name    = other.name;
        this->id      = other.id;
        this->parent  = other.parent;
    }


    triton::arch::registers_e RegisterSpec::getId(void) const {
      return this->id;
    }


    triton::uint32 RegisterSpec::getAbstractLow(void) const {
      return this->getLow();
    }


    triton::uint32 RegisterSpec::getAbstractHigh(void) const {
      return this->getHigh();
    }


    triton::arch::registers_e RegisterSpec::getParent(void) const {
      return this->parent;
    }


    triton::uint32 RegisterSpec::getBitSize(void) const {
      return this->getVectorSize();
    }


    triton::uint32 RegisterSpec::getSize(void) const {
      return this->getVectorSize() / BYTE_SIZE_BIT;
    }


    std::string RegisterSpec::getName(void) const {
      return this->name;
    }


    triton::uint32 RegisterSpec::getType(void) const {
      return triton::arch::OP_REG;
    }


    bool RegisterSpec::isOverlapWith(const RegisterSpec& other) const {
      if (this->parent == other.parent) {
        if (this->getLow() <= other.getLow() && other.getLow() <= this->getHigh()) return true;
        if (other.getLow() <= this->getLow() && this->getLow() <= other.getHigh()) return true;
      }
      return false;
    }


    bool RegisterSpec::operator==(const RegisterSpec& other) const {
      return getId() == other.getId();
    }


    bool RegisterSpec::operator!=(const RegisterSpec& other) const {
      return !(*this == other);
    }


    void RegisterSpec::operator=(const RegisterSpec& other) {
      BitsVector::operator=(other);
      this->name    = other.name;
      this->id      = other.id;
      this->parent  = other.parent;
    }


    std::ostream& operator<<(std::ostream& stream, const RegisterSpec& reg) {
      stream << reg.getName()
             << ":"
             << std::dec << reg.getBitSize()
             << " bv["
             << reg.getHigh()
             << ".."
             << reg.getLow()
             << "]";
      return stream;
    }


    std::ostream& operator<<(std::ostream& stream, const RegisterSpec* reg) {
      stream << *reg;
      return stream;
    }


    ////////////////////////////////////////////
    // Register
    ////////////////////////////////////////////

    Register::Register()
      : RegisterSpec(triton::arch::ID_REG_INVALID, "unknown", triton::arch::ID_REG_INVALID, 0, 0) {
    }


    Register::Register(const Register& other)
      : RegisterSpec(other) {
    }


    Register::Register(const RegisterSpec& spec)
      : RegisterSpec(spec) {
    }


    Register::Register(const triton::arch::CpuInterface& cpu, triton::arch::registers_e regId)
      : Register(
          (regId == triton::arch::ID_REG_INVALID) ?
          triton::arch::RegisterSpec(triton::arch::ID_REG_INVALID, "unknown", triton::arch::ID_REG_INVALID, 0, 0) : cpu.getRegister(regId)
        ) {
    }


    bool Register::operator==(const Register& other) const {
      return RegisterSpec::operator==(other);
    }


    bool Register::operator!=(const Register& other) const {
      return !(*this == other);
    }


    void Register::operator=(const Register& other) {
      RegisterSpec::operator=(other);
    }


    bool operator<(const Register& reg1, const Register& reg2) {
        return (reg1.getId() < reg2.getId());
    }

  }; /* arch namespace */
}; /* triton namespace */
