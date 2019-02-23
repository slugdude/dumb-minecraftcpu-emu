#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class RamPort;

class RegFile {
    
    protected:
        static std::vector<uint8_t> input;
        static std::vector<uint8_t> output;
        static uint8_t pc;
        
    public:
        
        
        static void write(uint8_t newVal, int reg) {
            input[reg] = newVal;
            //std::cout << "wrote " << static_cast<int>(newVal) << " to reg " << reg << std::endl;
        }
        
        static uint8_t read(int reg) {
            return output[reg];
        }
    
        
        virtual void cycle() { return; }
        virtual void write(uint8_t in) { return; }
        virtual uint8_t read() { return 0; }
        virtual RamPort* getRamPort() { return NULL; }
    
        static uint8_t getPC() {
            
            return pc;
            
        }
        
        static void incPC() {
            pc++;
        }
    
    
    
    
};

std::vector<uint8_t> RegFile::input = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
std::vector<uint8_t> RegFile::output = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t RegFile::pc = 1;

//0 delay slots
class MemReg : public RegFile {
    
    private:

        int regNum;
    
    public:
        
        MemReg(int setupRegNum) {
            
            if (setupRegNum < 0 or setupRegNum > 15) {
             
                setupRegNum = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            regNum = setupRegNum;
            
        }
        
        void cycle() {
            
            output[regNum] = input[regNum];
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum);
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum);
        }
        
        
};

//0 delay slots
class ShrReg : public RegFile {
    
    private:

        int regNum;
    
    public:
        
        ShrReg(int setupRegNum) {
            
            if (setupRegNum < 0 or setupRegNum > 15) {
             
                setupRegNum = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            regNum = setupRegNum;
            
        }
        
        void cycle() {
            
            output[regNum] = input[regNum] / 2;
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum);
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum);
        }
    
};

//1 delay slot
class PrimaryAddReg : public RegFile {
    
    private:

        int regNum1;
        int regNum2;
        
        int intercycle;
    
    public:
        
        PrimaryAddReg(int setupRegNum1, int setupRegNum2) {
            
            if (setupRegNum1 < 0 or setupRegNum1 > 15) {
             
                setupRegNum1 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            if (setupRegNum2 < 0 or setupRegNum2 > 15) {
             
                setupRegNum2 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            
            
            regNum1 = setupRegNum1;
            regNum2 = setupRegNum2;
            
        }
        
        void cycle() {
            
            output[regNum1] = intercycle;
            intercycle = input[regNum1] + input[regNum2];
            
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum1);
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum1);
        }
    
};

class NoOutReg : public RegFile {
    
    private:

        int regNum;
    
    public:
        
        NoOutReg(int setupRegNum) {
            
            if (setupRegNum < 0 or setupRegNum > 15) {
             
                setupRegNum = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            regNum = setupRegNum;
            
        }
        
        void cycle() {

        }
    
        void write(uint8_t in) {
            
            RegFile::write(in, regNum);
            
        }
        
        
        uint8_t read() {
            return 0;
        }
    
};

//1 delay slot
class SecondarySubReg : public RegFile {
    
    private:

        int regNum1;
        int regNum2;
        
        int intercycle;
    
    public:
        
        SecondarySubReg(int setupRegNum1, int setupRegNum2) {
            
            if (setupRegNum1 < 0 or setupRegNum1 > 15) {
             
                setupRegNum1 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            if (setupRegNum2 < 0 or setupRegNum2 > 15) {
             
                setupRegNum2 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            
            
            regNum1 = setupRegNum1;
            regNum2 = setupRegNum2;
            
        }
        
        void cycle() {
            
            output[regNum2] = intercycle;
            intercycle = input[regNum1] - input[regNum2];
            
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum2);
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum2);
        }
    
};

class ZeroReg : public RegFile {
    
    private:

        int regNum;
    
    public:
        
        ZeroReg(int setupRegNum) {
            
            if (setupRegNum < 0 or setupRegNum > 15) {
             
                setupRegNum = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            regNum = setupRegNum;
            RegFile::write(0, regNum);
            
        }
        
        void cycle() {
            
        }
        
        void write(uint8_t in) {
            
        }
        
        
        uint8_t read() {
            return 0;
        }
    
};

class PrimaryXnorReg : public RegFile {
    
    private:

        int regNum1;
        int regNum2;
        
        int intercycle;
    
    public:
        
        PrimaryXnorReg(int setupRegNum1, int setupRegNum2) {
            
            if (setupRegNum1 < 0 or setupRegNum1 > 15) {
             
                setupRegNum1 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            if (setupRegNum2 < 0 or setupRegNum2 > 15) {
             
                setupRegNum2 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            
            
            regNum1 = setupRegNum1;
            regNum2 = setupRegNum2;
            
        }
        
        void cycle() {
            
            output[regNum1] = intercycle;
            intercycle = input[regNum1] ^ input[regNum2];
            
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum1);
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum1);
        }   
    
};

class PrimaryNorReg : public RegFile {
    
    private:

        int regNum1;
        int regNum2;
        
        int intercycle;
    
    public:
        
        PrimaryNorReg(int setupRegNum1, int setupRegNum2) {
            
            if (setupRegNum1 < 0 or setupRegNum1 > 15) {
             
                setupRegNum1 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            if (setupRegNum2 < 0 or setupRegNum2 > 15) {
             
                setupRegNum2 = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            
            
            regNum1 = setupRegNum1;
            regNum2 = setupRegNum2;
            
        }
        
        void cycle() {
            
            output[regNum1] = intercycle;
            intercycle = ~(input[regNum1] | input[regNum2]);
            
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum1);
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum1);
        }   
    
};

class PrintMemReg : public RegFile {
    
    private:

        int regNum;
        bool print = false;
    
    public:
        
        PrintMemReg(int setupRegNum) {
            
            if (setupRegNum < 0 or setupRegNum > 15) {
             
                setupRegNum = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            regNum = setupRegNum;
            
        }
        
        void cycle() {
            
            output[regNum] = input[regNum];
            if (print) {
                
                std::cout << static_cast<int>(output[regNum]) << std::endl;
                print = false;
                
            }
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNum);
            print = true;
            
        }
        
        
        uint8_t read() {
            return RegFile::read(regNum);
        }
    
    
    
};


class BrzReg : public RegFile {
    
    private:

        int regNumCond;
        int regNumAddr;
        
        int intercycle[6];
        bool writebool[6] = {false, false, false, false, false, false};
        
    
    public:
        
        BrzReg(int setupRegNumCond, int setupRegNumAddr) {
            
            if (setupRegNumCond < 0 or setupRegNumCond > 15) {
             
                setupRegNumCond = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            if (setupRegNumAddr < 0 or setupRegNumAddr > 15) {
             
                setupRegNumAddr = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            
            regNumCond = setupRegNumCond;
            regNumAddr = setupRegNumAddr;
            
        }
        
        void cycle() {
            
            if (writebool[5] and intercycle[5] == 0) {
                pc = input[regNumAddr];
                //std::cout << "branch to " << static_cast<int>(input[regNumAddr]) << " ";
            }
            
            writebool[5] = writebool[4];
            writebool[4] = writebool[3];
            writebool[3] = writebool[2];
            writebool[2] = writebool[1];
            writebool[1] = writebool[0];
            writebool[0] = false;
            
            intercycle[5] = intercycle[4];
            intercycle[4] = intercycle[3];
            intercycle[3] = intercycle[2];
            intercycle[2] = intercycle[1];
            intercycle[1] = intercycle[0];
            intercycle[0] = input[regNumCond];
            
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, regNumAddr);
            writebool[0] = true;
            
        }
        
        
        uint8_t read() {
            return 0;
        } 
    
    
};

class RamPort {
    
    protected:
        
        uint8_t ram[256];
        
    public:
        
        RamPort() {
            
        }
        
    
        int RegNumAddr;
        int RegNumData;
        
        void writeRam(uint8_t data, uint8_t addr) {
            
            ram[addr] = data;
            
            
        }
        
        uint8_t getRam(int addr) {
         
            return ram[addr];
            
        }
    
};

class RamReadReg : public RegFile {
    
    private:
        RamPort* RamObj;
    
    public:
        RamReadReg(int setupAddr) {
            
            if (setupAddr < 0 or setupAddr > 15) {
             
                setupAddr = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            RamObj = new RamPort;
            
            RamObj->RegNumAddr = setupAddr;
            
            
        }
        
        ~RamReadReg() {
            delete RamObj;
        }
    
        
        void cycle() {
            
            output[RamObj->RegNumAddr] = RamObj->getRam(input[RamObj->RegNumAddr]);
            
        }
        
        void write(uint8_t in) {
            
            RegFile::write(in, RamObj->RegNumAddr);
            
        }
        
        
        uint8_t read() {
            return output[RamObj->RegNumAddr];
        }
        
        RamPort* getRamPort() {
            return RamObj;
        }
    
    
};

class RamWriteReg : public RegFile {
    
    private:
        RamPort* RamObj;
        bool writeNext = false;
        
    public:
        RamWriteReg(int setupData, RamPort* RamObjPtr) {
            
            if (setupData < 0 or setupData > 15) {
             
                setupData = 0;
                std::cerr << "Error: Bad config, register index out of bounds, using 0." << std::endl;
                
            }
            
            RamObj = RamObjPtr;
            RamObj->RegNumData = setupData;
            
            
        }
        
        void cycle() {
            
            if (writeNext) {
                    
                //RamObj->ram[input[RamObj->RegNumAddr]] = input[RamObj->RegNumData];
                
                RamObj->writeRam(input[RamObj->RegNumData], input[RamObj->RegNumAddr]);
                writeNext = false;
                
                output[RamObj->RegNumAddr] = RamObj->getRam(input[RamObj->RegNumAddr]);
                
            }
            
            
        }
        
        void write(uint8_t in) {
         
            RegFile::write(in, RamObj->RegNumData);
            writeNext = true;
             
        }
        
        uint8_t read() {
            
            return 0;
            
        }
        
        
};
        

void cycleAll(RegFile** regs) {
    
    for (int i = 0; i < 16; i++) {
        
        regs[i]->cycle();
        
    }
    
}

int main(int argc, char *argv[]) {
    
    if(argc <= 1) {
        
        std::cout << "No binary file. Exiting." << std::endl;
        return 0;
        
    }
    
    std::string binFileName = argv[1];
    
    std::ifstream binFile;
    
    binFile.open(binFileName.c_str(), std::fstream::binary | std::fstream::out);
    
    if(!binFile.is_open()){
        
        std::cout << "File does not exist. Exiting." << std::endl;
        return 0;
        
    }
    
    std::vector<uint16_t> words;
    
    uint8_t prev = 0;
    
    char* input = new char[322];
    
    binFile.read(input, 322);
    
    uint16_t* test = new uint16_t[322];
    for (int i = 0; i < 322; i++) {
        
        test[i] = input[i];
        
    }
    
    words.push_back(0);
    
    for (int i = 1; i < 321; i++) {
        switch ((i - 1) % 5) {
            case 0:
                words.push_back(((test[i-1] & 0b11111111) << 2) | ((test[i] & 0b11000000) >> 6));
                break;
            case 1:
                words.push_back(((test[i-1] & 0b00111111) << 4) | ((test[i] & 0b11110000) >> 4));
                break;
            case 2:
                words.push_back(((test[i-1] & 0b00001111) << 6) | ((test[i] & 0b11111100) >> 2));
                break;
            case 3:
                words.push_back(((test[i-1] & 0b00000011) << 8) | (test[i] & 0b11111111));
                i++;
                break;
                
        }
        
    }

    binFile.close();
    
    RegFile* regs[16];
    
    regs[0] = new ZeroReg(0);
    regs[1] = new PrintMemReg(1);
    regs[2] = new MemReg(2);
    regs[3] = new MemReg(3);
    regs[4] = new MemReg(4);
    regs[5] = new MemReg(5);
    regs[6] = new PrimaryNorReg(6, 7);
    regs[7] = new PrimaryXnorReg(7, 6);
    regs[8] = new PrimaryAddReg(8, 9);
    regs[9] = new NoOutReg(9);
    regs[10] = new SecondarySubReg(9, 10);
    regs[11] = new ShrReg(11);
    regs[12] = new NoOutReg(12);
    regs[13] = new BrzReg(12, 13);
    
    
    
    regs[14] = new RamReadReg(14);
    regs[15] = new RamWriteReg(15, regs[14]->getRamPort());
    
    
    int count = 0;
    
    int i = 1;
    
    while (i != 0 and count < 50) {
        
        i = RegFile::getPC();
        
        //std::cout << std::endl << "pc: " << i << std::endl;
        
        RegFile::incPC();
        
        int dest = (words[i] & 0b0011110000) >> 4;
        int source = (words[i] & 0b0000001111);

        if ((words[i] & 0b1000000000) == 0b1000000000) {
            //CPY op
            
            
            regs[dest]->write(regs[source]->read());
            
            
            
        } else {
            //LDI op
            
            int imm;
            if ((words[i] & 0b0100000000) == 0) {
                imm = source;
            } else {
                imm = source << 4;
            }
            regs[dest]->write(imm);
            
            
            
        }
        
        cycleAll(regs);
        count++;
        
    }
    
    if (count >= 50) std::cout << "count reached" << std::endl;
    
    
    for (int i = 0; i < 16; i++) {
        
        delete regs[i];
        
    }
    
    //delete ram;
    
    return 0;
    
    
}
