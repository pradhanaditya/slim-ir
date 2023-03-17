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
#include <set>

// Types of SLIM operands
typedef enum
{
    VARIABLE,
    GEP_OPERATOR,
    ADDR_SPACE_CAST_OPERATOR,
    BITCAST_OPERATOR,
    PTR_TO_INT_OPERATOR,
    ZEXT_OPERATOR,
    FP_MATH_OPERATOR,
    BLOCK_ADDRESS,
    CONSTANT_AGGREGATE,
    CONSTANT_DATA_SEQUENTIAL,
    CONSTANT_POINTER_NULL,
    CONSTANT_TOKEN_NONE,
    UNDEF_VALUE,
    CONSTANT_INT,
    CONSTANT_FP,
    DSO_LOCAL_EQUIVALENT,
    GLOBAL_VALUE,
    NO_CFI_VALUE,
    NOT_SUPPORTED_OPERAND,
    NULL_OPERAND
} OperandType;

// Holds operand and some other useful information
class SLIMOperand
{
protected:
    // Type of the operand
    OperandType operand_type;

    // LLVM value object corresponding to the operand
    llvm::Value *value;
    
    // llvm::Value * of the main aggregate (if the operand is a GEPOperator)
    llvm::Value *gep_main_operand;

    // Is the operand a global variable or an address-taken local variable
    bool is_global_or_address_taken;

    // Is the operand a pointer variable
    bool is_pointer_variable;

    // Does the operand contain indices
    bool has_indices;

    // Does the operand have a name
    bool has_name;

    // Set the SSA version
    bool is_ssa_version;
    unsigned ssa_version_number;

    // Contains the llvm::Value * object corresponding to the indices present in the operand
    std::vector<SLIMOperand *> indices;

private:
    // Internal function to be used only in case of print related tasks
    std::string _getOperandName();

    // Internal function which returns the operand type of a llvm::Value object
    static OperandType processOperand(llvm::Value *value);

public:
    // Constructors
    SLIMOperand(llvm::Value *value);
    SLIMOperand(llvm::Value *value, bool is_global_or_address_taken);

    // Returns the operand type
    OperandType getOperandType();
        
    // Returns true if the operand is a global variable or an address-taken local variable
    bool isGlobalOrAddressTaken();

    // Returns true if the operand is a result of an alloca instruction
    bool isAlloca();

    // Returns true if the operand is a pointer variable (with reference to the LLVM IR)
    bool isPointerInLLVM();
    
    // Returns true if the operand is a pointer variable (with reference to the source program)
    bool isPointerVariable();

    // Returns true if the operand is of array type
    bool isArray();
    
    // Returns true if the operand is a result of GetElementPtr instruction
    bool isResultOfGEP();
    
    // Sets the is_pointer_variable to true
    void setIsPointerVariable();

    // Sets the is_pointer_variable to false
    void unsetIsPointerVariable();

    // Returns the pointer to the corresponding llvm::Value object 
    llvm::Value * getValue();

    // Returns the number of indices
    unsigned getNumIndices();

    // Does the operand have a name
    bool hasName();
    
    // Returns the operand index at the specified position (0-based position)
    SLIMOperand * getIndexOperand(unsigned position);

    void addIndexOperand(SLIMOperand *indOperand);

    // Prints the SLIM operand
    void printOperand(llvm::raw_ostream &stream);

    // Sets the SSA version
    void setSSAVersion(unsigned ssa_version);

    // Reset the SSA version
    void resetSSAVersion();

    // --------------- APIs for the Legacy SLIM ---------------
    
    // Returns the name of the operand
    llvm::StringRef getName();
    
    // --------------------------------------------------------
};

namespace OperandRepository
{
    // Check whether a SLIMOperand object corresponds to a global or a address-taken local variable or not
    extern std::map<llvm::Value *, SLIMOperand *> value_to_slim_operand;

    // Contains the value objects that are a result of alloca instruction
    extern std::set<llvm::Value *> alloca_operand;

    // Returns the SLIMOperand object if already exists, otherwise returns a nullptr
    SLIMOperand * getSLIMOperand(llvm::Value *value);

    // Set the SLIMOperand object corresponding to a LLVM Value object
    void setSLIMOperand(llvm::Value *value, SLIMOperand *slim_operand);
};