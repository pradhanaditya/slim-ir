#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/CFG.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APInt.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringRef.h"
#include <string>
#include <map>

// Holds operand and some other useful information
class SLIMOperand
{
protected:
    llvm::Value *value;
    
    // Is the operand a global variable or an address-taken local variable
    bool is_global_or_address_taken;

    // Is the operand a pointer variable
    bool is_pointer_variable;

private:
    // Internal function to be used only in case of print related tasks
    std::string _getOperandName();

public:
    // Constructors
    SLIMOperand(llvm::Value *value);
    SLIMOperand(llvm::Value *value, bool is_global_or_address_taken);
    
    // Returns true if the operand is a global variable or an address-taken local variable
    bool isGlobalOrAddressTaken();

    // Returns true if the operand is a pointer variable (with reference to the source program)
    bool isPointerVariable();

    // Sets the is_pointer_variable to true
    void setIsPointerVariable();

    // Sets the is_pointer_variable to false
    void unsetIsPointerVariable();

    // Returns the pointer to the corresponding llvm::Value object 
    llvm::Value * getValue();

    // Prints the SLIM operand
    void printOperand(llvm::raw_ostream &stream);

    // --------------- APIs for the Legacy SLIM ---------------
    
    // Returns the name of the operand
    llvm::StringRef getName();
    
    // --------------------------------------------------------
};

namespace OperandRepository
{
    // Check whether a SLIMOperand object corresponds to a global or a address-taken local variable or not
    extern std::map<llvm::Value *, SLIMOperand *> value_to_slim_operand;

    // Returns the SLIMOperand object if already exists, otherwise returns a nullptr
    SLIMOperand * getSLIMOperand(llvm::Value *value);

    // Set the SLIMOperand object corresponding to a LLVM Value object
    void setSLIMOperand(llvm::Value *value, SLIMOperand *slim_operand);
};