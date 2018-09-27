#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
using std::cout;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;

typedef vector<byte> bin_array;
typedef vector<byte>::iterator bin_array_it;
typedef vector<byte>::const_iterator bin_array_const_it;

int16_t dissassemble8080Instruction(bin_array_const_it & it, int pc) {
  byte opcode = *it;
  int opbytes = 1;

  cout << hex << setfill('0');
  cout << setw(4) << pc << " " << setw(2) << std::to_integer<int>(opcode) << " ";
  // cout << std::left << setfill(' ') << setw();
  switch(std::to_integer<unsigned char>(opcode)) {
    case 0x00 : cout << "NOP        " ; opbytes = 1; break;
    case 0x01 : cout << "LXI    B,#$" ; opbytes = 3; break;
    case 0x02 : cout << "STAX   B   " ; opbytes = 1; break;
    case 0x03 : cout << "INX    B   " ; opbytes = 1; break;
    case 0x04 : cout << "INR    B   " ; opbytes = 1; break;
    case 0x05 : cout << "DCR    B   " ; opbytes = 1; break;
    case 0x06 : cout << "MVI    B,#$" ; opbytes = 2; break;
    case 0x07 : cout << "RLC        " ; opbytes = 1; break;
    case 0x08 : cout << "NOP        " ; opbytes = 1; break;
    case 0x09 : cout << "DAD    B   " ; opbytes = 1; break;
    case 0x0A : cout << "LDAX   B   " ; opbytes = 1; break;
    case 0x0B : cout << "DCX    B   " ; opbytes = 1; break;
    case 0x0C : cout << "INR    C   " ; opbytes = 1; break;
    case 0x0D : cout << "DCR    C   " ; opbytes = 1; break;
    case 0x0E : cout << "MVI    C,#$" ; opbytes = 2; break;
    case 0x0F : cout << "RRC        " ; opbytes = 1; break;

    case 0x10 : cout << "NOP        " ; opbytes = 1; break;
    case 0x11 : cout << "LXI    D,#$" ; opbytes = 3; break;
    case 0x12 : cout << "STAX   D   " ; opbytes = 1; break;
    case 0x13 : cout << "INX    D   " ; opbytes = 1; break;
    case 0x14 : cout << "INR    D   " ; opbytes = 1; break;
    case 0x15 : cout << "DCR    D   " ; opbytes = 1; break;
    case 0x16 : cout << "MVI    D,#$" ; opbytes = 2; break;
    case 0x17 : cout << "RAL        " ; opbytes = 1; break;
    case 0x18 : cout << "NOP        " ; opbytes = 1; break;
    case 0x19 : cout << "DAD    D   " ; opbytes = 1; break;
    case 0x1A : cout << "LDAX   D   " ; opbytes = 1; break;
    case 0x1B : cout << "DCX    D   " ; opbytes = 1; break;
    case 0x1C : cout << "INR    E   " ; opbytes = 1; break;
    case 0x1D : cout << "DCR    E   " ; opbytes = 1; break;
    case 0x1E : cout << "MVI    E,#$" ; opbytes = 2; break;
    case 0x1F : cout << "RAC        " ; opbytes = 1; break;

    case 0x20 : cout << "NOP        " ; opbytes = 1; break;
    case 0x21 : cout << "LXI    H,#$" ; opbytes = 3; break;
    case 0x22 : cout << "SHLD   $"    ; opbytes = 3; break;
    case 0x23 : cout << "INX    H   " ; opbytes = 1; break;
    case 0x24 : cout << "INR    H   " ; opbytes = 1; break;
    case 0x25 : cout << "DCR    H   " ; opbytes = 1; break;
    case 0x26 : cout << "MVI    H,#$" ; opbytes = 2; break;
    case 0x27 : cout << "DAA        " ; opbytes = 1; break;
    case 0x28 : cout << "NOP        " ; opbytes = 1; break;
    case 0x29 : cout << "DAD    H   " ; opbytes = 1; break;
    case 0x2A : cout << "LHLD   $" ;    opbytes = 3; break;
    case 0x2B : cout << "DCX    H   " ; opbytes = 1; break;
    case 0x2C : cout << "INR    L   " ; opbytes = 1; break;
    case 0x2D : cout << "DCR    L   " ; opbytes = 1; break;
    case 0x2E : cout << "MVI    L,#$" ; opbytes = 2; break;
    case 0x2F : cout << "CMA        " ; opbytes = 1; break;

    case 0x30 : cout << "NOP        " ; opbytes = 1; break;
    case 0x31 : cout << "LXI    SP,#$"; opbytes = 3; break;
    case 0x32 : cout << "STA    $"    ; opbytes = 3; break;
    case 0x33 : cout << "INX    SP  " ; opbytes = 1; break;
    case 0x34 : cout << "INR    M   " ; opbytes = 1; break;
    case 0x35 : cout << "DCR    M   " ; opbytes = 1; break;
    case 0x36 : cout << "MVI    M,#$" ; opbytes = 2; break;
    case 0x37 : cout << "STC        " ; opbytes = 1; break;
    case 0x38 : cout << "NOP        " ; opbytes = 1; break;
    case 0x39 : cout << "DAD    SP  " ; opbytes = 1; break;
    case 0x3A : cout << "LDA    $"    ; opbytes = 3; break;
    case 0x3B : cout << "DCX    SP  " ; opbytes = 1; break;
    case 0x3C : cout << "INR    A   " ; opbytes = 1; break;
    case 0x3D : cout << "DCR    A   " ; opbytes = 1; break;
    case 0x3E : cout << "MVI    A,#$" ; opbytes = 2; break;
    case 0x3F : cout << "CMC        " ; opbytes = 1; break;

    case 0x40 : cout << "MOV    B,B " ; opbytes = 1; break;
    case 0x41 : cout << "MOV    B,C " ; opbytes = 1; break;
    case 0x42 : cout << "MOV    B,D " ; opbytes = 1; break;
    case 0x43 : cout << "MOV    B,E " ; opbytes = 1; break;
    case 0x44 : cout << "MOV    B,H " ; opbytes = 1; break;
    case 0x45 : cout << "MOV    B,L " ; opbytes = 1; break;
    case 0x46 : cout << "MOV    B,M " ; opbytes = 1; break;
    case 0x47 : cout << "MOV    B,A " ; opbytes = 1; break;
    case 0x48 : cout << "MOV    C,B " ; opbytes = 1; break;
    case 0x49 : cout << "MOV    C,C " ; opbytes = 1; break;
    case 0x4A : cout << "MOV    C,D " ; opbytes = 1; break;
    case 0x4B : cout << "MOV    C,E " ; opbytes = 1; break;
    case 0x4C : cout << "MOV    C,H " ; opbytes = 1; break;
    case 0x4D : cout << "MOV    C,L " ; opbytes = 1; break;
    case 0x4E : cout << "MOV    C,M " ; opbytes = 1; break;
    case 0x4F : cout << "MOV    C,A " ; opbytes = 1; break;

    case 0x50 : cout << "MOV    D,B " ; opbytes = 1; break;
    case 0x51 : cout << "MOV    D,C " ; opbytes = 1; break;
    case 0x52 : cout << "MOV    D,D " ; opbytes = 1; break;
    case 0x53 : cout << "MOV    D,E " ; opbytes = 1; break;
    case 0x54 : cout << "MOV    D,H " ; opbytes = 1; break;
    case 0x55 : cout << "MOV    D,L " ; opbytes = 1; break;
    case 0x56 : cout << "MOV    D,M " ; opbytes = 1; break;
    case 0x57 : cout << "MOV    D,A " ; opbytes = 1; break;
    case 0x58 : cout << "MOV    E,B " ; opbytes = 1; break;
    case 0x59 : cout << "MOV    E,C " ; opbytes = 1; break;
    case 0x5A : cout << "MOV    E,D " ; opbytes = 1; break;
    case 0x5B : cout << "MOV    E,E " ; opbytes = 1; break;
    case 0x5C : cout << "MOV    E,H " ; opbytes = 1; break;
    case 0x5D : cout << "MOV    E,L " ; opbytes = 1; break;
    case 0x5E : cout << "MOV    E,M " ; opbytes = 1; break;
    case 0x5F : cout << "MOV    E,A " ; opbytes = 1; break;

    case 0x60 : cout << "MOV    H,B " ; opbytes = 1; break;
    case 0x61 : cout << "MOV    H,C " ; opbytes = 1; break;
    case 0x62 : cout << "MOV    H,D " ; opbytes = 1; break;
    case 0x63 : cout << "MOV    H,E " ; opbytes = 1; break;
    case 0x64 : cout << "MOV    H,H " ; opbytes = 1; break;
    case 0x65 : cout << "MOV    H,L " ; opbytes = 1; break;
    case 0x66 : cout << "MOV    H,M " ; opbytes = 1; break;
    case 0x67 : cout << "MOV    H,A " ; opbytes = 1; break;
    case 0x68 : cout << "MOV    L,B " ; opbytes = 1; break;
    case 0x69 : cout << "MOV    L,C " ; opbytes = 1; break;
    case 0x6A : cout << "MOV    L,D " ; opbytes = 1; break;
    case 0x6B : cout << "MOV    L,E " ; opbytes = 1; break;
    case 0x6C : cout << "MOV    L,H " ; opbytes = 1; break;
    case 0x6D : cout << "MOV    L,L " ; opbytes = 1; break;
    case 0x6E : cout << "MOV    L,M " ; opbytes = 1; break;
    case 0x6F : cout << "MOV    L,A " ; opbytes = 1; break;

    case 0x70 : cout << "MOV    M,B " ; opbytes = 1; break;
    case 0x71 : cout << "MOV    M,C " ; opbytes = 1; break;
    case 0x72 : cout << "MOV    M,D " ; opbytes = 1; break;
    case 0x73 : cout << "MOV    M,E " ; opbytes = 1; break;
    case 0x74 : cout << "MOV    M,H " ; opbytes = 1; break;
    case 0x75 : cout << "MOV    M,L " ; opbytes = 1; break;
    case 0x76 : cout << "HLT        " ; opbytes = 1; break;
    case 0x77 : cout << "MOV    M,A " ; opbytes = 1; break;
    case 0x78 : cout << "MOV    A,B " ; opbytes = 1; break;
    case 0x79 : cout << "MOV    A,C " ; opbytes = 1; break;
    case 0x7A : cout << "MOV    A,D " ; opbytes = 1; break;
    case 0x7B : cout << "MOV    A,E " ; opbytes = 1; break;
    case 0x7C : cout << "MOV    A,H " ; opbytes = 1; break;
    case 0x7D : cout << "MOV    A,L " ; opbytes = 1; break;
    case 0x7E : cout << "MOV    A,M " ; opbytes = 1; break;
    case 0x7F : cout << "MOV    A,A " ; opbytes = 1; break;

    case 0x80 : cout << "ADD    B   " ; opbytes = 1; break;
    case 0x81 : cout << "ADD    C   " ; opbytes = 1; break;
    case 0x82 : cout << "ADD    D   " ; opbytes = 1; break;
    case 0x83 : cout << "ADD    E   " ; opbytes = 1; break;
    case 0x84 : cout << "ADD    H   " ; opbytes = 1; break;
    case 0x85 : cout << "ADD    L   " ; opbytes = 1; break;
    case 0x86 : cout << "ADD    M   " ; opbytes = 1; break;
    case 0x87 : cout << "ADD    A   " ; opbytes = 1; break;
    case 0x88 : cout << "ADC    B   " ; opbytes = 1; break;
    case 0x89 : cout << "ADC    C   " ; opbytes = 1; break;
    case 0x8A : cout << "ADC    D   " ; opbytes = 1; break;
    case 0x8B : cout << "ADC    E   " ; opbytes = 1; break;
    case 0x8C : cout << "ADC    H   " ; opbytes = 1; break;
    case 0x8D : cout << "ADC    L   " ; opbytes = 1; break;
    case 0x8E : cout << "ADC    M   " ; opbytes = 1; break;
    case 0x8F : cout << "ADC    A   " ; opbytes = 1; break;

    case 0x90 : cout << "SUB    B   " ; opbytes = 1; break;
    case 0x91 : cout << "SUB    C   " ; opbytes = 1; break;
    case 0x92 : cout << "SUB    D   " ; opbytes = 1; break;
    case 0x93 : cout << "SUB    E   " ; opbytes = 1; break;
    case 0x94 : cout << "SUB    H   " ; opbytes = 1; break;
    case 0x95 : cout << "SUB    L   " ; opbytes = 1; break;
    case 0x96 : cout << "SUB    M   " ; opbytes = 1; break;
    case 0x97 : cout << "SUB    A   " ; opbytes = 1; break;
    case 0x98 : cout << "SBB    B   " ; opbytes = 1; break;
    case 0x99 : cout << "SBB    C   " ; opbytes = 1; break;
    case 0x9A : cout << "SBB    D   " ; opbytes = 1; break;
    case 0x9B : cout << "SBB    E   " ; opbytes = 1; break;
    case 0x9C : cout << "SBB    H   " ; opbytes = 1; break;
    case 0x9D : cout << "SBB    L   " ; opbytes = 1; break;
    case 0x9E : cout << "SBB    M   " ; opbytes = 1; break;
    case 0x9F : cout << "SBB    A   " ; opbytes = 1; break;

    case 0xA0 : cout << "ANA    B   " ; opbytes = 1; break;
    case 0xA1 : cout << "ANA    C   " ; opbytes = 1; break;
    case 0xA2 : cout << "ANA    D   " ; opbytes = 1; break;
    case 0xA3 : cout << "ANA    E   " ; opbytes = 1; break;
    case 0xA4 : cout << "ANA    H   " ; opbytes = 1; break;
    case 0xA5 : cout << "ANA    L   " ; opbytes = 1; break;
    case 0xA6 : cout << "ANA    M   " ; opbytes = 1; break;
    case 0xA7 : cout << "ANA    A   " ; opbytes = 1; break;
    case 0xA8 : cout << "XRA    B   " ; opbytes = 1; break;
    case 0xA9 : cout << "XRA    C   " ; opbytes = 1; break;
    case 0xAA : cout << "XRA    D   " ; opbytes = 1; break;
    case 0xAB : cout << "XRA    E   " ; opbytes = 1; break;
    case 0xAC : cout << "XRA    H   " ; opbytes = 1; break;
    case 0xAD : cout << "XRA    L   " ; opbytes = 1; break;
    case 0xAE : cout << "XRA    M   " ; opbytes = 1; break;
    case 0xAF : cout << "XRA    A   " ; opbytes = 1; break;

    case 0xB0 : cout << "ORA    B   " ; opbytes = 1; break;
    case 0xB1 : cout << "ORA    C   " ; opbytes = 1; break;
    case 0xB2 : cout << "ORA    D   " ; opbytes = 1; break;
    case 0xB3 : cout << "ORA    E   " ; opbytes = 1; break;
    case 0xB4 : cout << "ORA    H   " ; opbytes = 1; break;
    case 0xB5 : cout << "ORA    L   " ; opbytes = 1; break;
    case 0xB6 : cout << "ORA    M   " ; opbytes = 1; break;
    case 0xB7 : cout << "ORA    A   " ; opbytes = 1; break;
    case 0xB8 : cout << "CMP    B   " ; opbytes = 1; break;
    case 0xB9 : cout << "CMP    C   " ; opbytes = 1; break;
    case 0xBA : cout << "CMP    D   " ; opbytes = 1; break;
    case 0xBB : cout << "CMP    E   " ; opbytes = 1; break;
    case 0xBC : cout << "CMP    H   " ; opbytes = 1; break;
    case 0xBD : cout << "CMP    L   " ; opbytes = 1; break;
    case 0xBE : cout << "CMP    M   " ; opbytes = 1; break;
    case 0xBF : cout << "CMP    A   " ; opbytes = 1; break;

    case 0xC0 : cout << "RNZ        " ; opbytes = 1; break;
    case 0xC1 : cout << "POP    B   " ; opbytes = 1; break;
    case 0xC2 : cout << "JNZ    $"    ; opbytes = 3; break;
    case 0xC3 : cout << "JMP    $"    ; opbytes = 3; break;
    case 0xC4 : cout << "CNZ    $"    ; opbytes = 3; break;
    case 0xC5 : cout << "PUSH   B   " ; opbytes = 1; break;
    case 0xC6 : cout << "ADI    #$"   ; opbytes = 2; break;
    case 0xC7 : cout << "RST    0   " ; opbytes = 1; break;
    case 0xC8 : cout << "RZ         " ; opbytes = 1; break;
    case 0xC9 : cout << "RET        " ; opbytes = 1; break;
    case 0xCA : cout << "JZ     $"    ; opbytes = 3; break;
    case 0xCB : cout << "JMP    $"    ; opbytes = 3; break;
    case 0xCC : cout << "CZ     $"    ; opbytes = 3; break;
    case 0xCD : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xCE : cout << "ACI    $"    ; opbytes = 2; break;
    case 0xCF : cout << "RST    1   " ; opbytes = 1; break;

    case 0xD0 : cout << "RNC        " ; opbytes = 1; break;
    case 0xD1 : cout << "POP    D   " ; opbytes = 1; break;
    case 0xD2 : cout << "JNC    $"    ; opbytes = 3; break;
    case 0xD3 : cout << "OUT    $"    ; opbytes = 2; break;
    case 0xD4 : cout << "CNC    $"    ; opbytes = 3; break;
    case 0xD5 : cout << "PUSH   D   " ; opbytes = 1; break;
    case 0xD6 : cout << "SUI    #$"   ; opbytes = 2; break;
    case 0xD7 : cout << "RST    2   " ; opbytes = 1; break;
    case 0xD8 : cout << "RC         " ; opbytes = 1; break;
    case 0xD9 : cout << "RET        " ; opbytes = 1; break;
    case 0xDA : cout << "JC     $"    ; opbytes = 3; break;
    case 0xDB : cout << "IN     $"    ; opbytes = 2; break;
    case 0xDC : cout << "CC     $"    ; opbytes = 3; break;
    case 0xDD : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xDE : cout << "SBI    $"    ; opbytes = 2; break;
    case 0xDF : cout << "RST    3   " ; opbytes = 1; break;

    case 0xE0 : cout << "RPO        " ; opbytes = 1; break;
    case 0xE1 : cout << "POP    H   " ; opbytes = 1; break;
    case 0xE2 : cout << "JPO    $"    ; opbytes = 3; break;
    case 0xE3 : cout << "XTHL       " ; opbytes = 1; break;
    case 0xE4 : cout << "CPO    $"    ; opbytes = 3; break;
    case 0xE5 : cout << "PUSH   H   " ; opbytes = 1; break;
    case 0xE6 : cout << "ANI    #$"   ; opbytes = 2; break;
    case 0xE7 : cout << "RST    4   " ; opbytes = 1; break;
    case 0xE8 : cout << "RPE        " ; opbytes = 1; break;
    case 0xE9 : cout << "PCHL       " ; opbytes = 1; break;
    case 0xEA : cout << "JPE    $"    ; opbytes = 3; break;
    case 0xEB : cout << "XCHG       " ; opbytes = 1; break;
    case 0xEC : cout << "CPE    $"    ; opbytes = 3; break;
    case 0xED : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xEE : cout << "XRI    $"    ; opbytes = 2; break;
    case 0xEF : cout << "RST    5   " ; opbytes = 1; break;

    case 0xF0 : cout << "RP         " ; opbytes = 1; break;
    case 0xF1 : cout << "POP    PSW " ; opbytes = 1; break;
    case 0xF2 : cout << "JP     $"    ; opbytes = 3; break;
    case 0xF3 : cout << "DI         " ; opbytes = 1; break;
    case 0xF4 : cout << "CP     $"    ; opbytes = 3; break;
    case 0xF5 : cout << "PUSH   PSW " ; opbytes = 1; break;
    case 0xF6 : cout << "ORI    #$"   ; opbytes = 2; break;
    case 0xF7 : cout << "RST    6   " ; opbytes = 1; break;
    case 0xF8 : cout << "RM         " ; opbytes = 1; break;
    case 0xF9 : cout << "SPHL       " ; opbytes = 1; break;
    case 0xFA : cout << "JM     $"    ; opbytes = 3; break;
    case 0xFB : cout << "EI         " ; opbytes = 1; break;
    case 0xFC : cout << "CM     $"    ; opbytes = 3; break;
    case 0xFD : cout << "CALL   $"    ; opbytes = 3; break;
    case 0xFE : cout << "CPI    $"    ; opbytes = 2; break;
    case 0xFF : cout << "RST    7   " ; opbytes = 1; break;

    default   : cout << "UNK"; break;
  }

  if (opbytes == 3) {
    auto b2 = std::to_integer<int>(*(it+2));
    auto b1 = std::to_integer<int>(*(it+1));
    cout << hex << setfill('0');
    cout  << setw(2) << b2  << setw(2) << b1;
  } else if (opbytes == 2) {
    auto b1 = std::to_integer<int>(*(it+1));
    cout << hex << setfill('0') << setw(2);
    cout << b1;
  }

  // it+=opbytes;
  cout << endl;
  return opbytes;
}

void dissassemble8080Blob(bin_array const& binaryCode) {
  // cout << hex << setfill('0');  // needs to be set only once
  auto it = binaryCode.begin();
  while (it != binaryCode.end()) {
    auto opbytes = dissassemble8080Instruction(it, it - binaryCode.begin());
    it+=opbytes; 
  }
}

bin_array load_file(std::string const& filepath) {
  std::ifstream ifs(filepath, std::ios::binary|std::ios::ate);
  if (!ifs) throw std::runtime_error(filepath + ": " + std::strerror(errno));

  auto  end = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  auto size = std::size_t(end - ifs.tellg());
  if (size == 0) return {};

  bin_array buffer(size);
  if (!ifs.read((char*) buffer.data(), buffer.size())) {
    throw std::runtime_error(filepath + ": " + std::strerror(errno));
  }

  return buffer;
}

int main (int argc, char**argv) {
  auto file = string(argv[1]);
  auto fileContent = load_file(file);
  std::cout << "File content size is: " << fileContent.size() << std::endl;
  dissassemble8080Blob(fileContent);
  return 0;
}
