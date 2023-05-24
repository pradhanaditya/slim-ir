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

    // Holds the value "true" if the operand corresponds to the formal argument of a function
    bool is_formal_argument;

    // Is the operand a pointer variable
    bool is_pointer_variable;

    // Does the operand contain indices
    bool has_indices;

    // Does the operand have a name
    bool has_name;

    // Set the SSA version
    bool is_ssa_version;
    unsigned ssa_version_number;

    // To manually keep track whether the operand is of array type
    bool is_array_type;

    // Contains the llvm::Value * object corresponding to the indices present in the operand
    std::vector<SLIMOperand *> indices;

    // Contains pointer to the function object (corresponding to the callee) if the operand is the result of a call instruction 
    llvm::Function *direct_callee_function;

private:
    // Internal function to be used only in case of print related tasks
    std::string _getOperandName();

    // Internal function which returns the operand type of a llvm::Value object
    static OperandType processOperand(llvm::Value *value);

public:
    // Constructors
    SLIMOperand(llvm::Value *value);
    SLIMOperand(llvm::Value *value, bool is_global_or_address_taken, llvm::Function *direct_callee_function = nullptr);

    // Returns the operand type
    OperandType getOperandType();
        
    // Returns true if the operand is a global variable or an address-taken local variable
    bool isGlobalOrAddressTaken();

    // Returns true if the operand is a formal argument of a function
    bool isFormalArgument();

    // Returns true if the operand is a global variable or an address-taken local variable (considers only the struct if the operand is a GEP operator)
    bool isVariableGlobal();

    // Returns true if the operand is a result of an alloca instruction
    bool isAlloca();

    // Returns true if the operand is a pointer variable (with reference to the LLVM IR)
    bool isPointerInLLVM();
    
    // Returns true if the operand is a pointer variable (with reference to the source program)
    bool isPointerVariable();

    // Returns true if the operand is of array type
    bool isArrayElement();
    
    // Sets the is_array_type to true
    void setArrayType();
    
    // Sets the gep_main_operand
    void setGEPMainOperand(SLIMOperand *operand);
    
    // Returns true if the operand is a GetElementPtr operand inside an instruction
    bool isGEPInInstr();
    
    // Sets the is_pointer_variable to true
    void setIsPointerVariable();

    // Sets the is_pointer_variable to false
    void unsetIsPointerVariable();

    // Returns the pointer to the corresponding llvm::Value object 
    llvm::Value * getValue();

    // Returns the type of the operand
    llvm::Type * getType();
    
    // Returns the number of indices
    unsigned getNumIndices();

    // Does the operand have a name
    bool hasName();
    
    // Returns the operand index at the specified position (0-based position)
    SLIMOperand * getIndexOperand(unsigned position);

    // Returns the vector of indices
    std::vector<SLIMOperand *> getIndexVector();

    void addIndexOperand(SLIMOperand *indOperand);

    // Prints the SLIM operand
    void printOperand(llvm::raw_ostream &stream);

    // Sets the SSA version
    void setSSAVersion(unsigned ssa_version);

    // Reset the SSA version
    void resetSSAVersion();

    // Returns the "return operand" of the callee function if this operand is the result of a "direct" call instruction
    SLIMOperand * getCalleeReturnOperand();

    // Clears the index vector
    void resetIndexVector();

    // Sets 'is_global_or_address_taken' to be true for this operand
    void setVariableGlobal();
    
    // Sets 'is_formal_argument' to be true for this operand
    void setFormalArgument();

    // --------------------------- APIs for the Legacy SLIM ---------------------------
    
    // Returns the name of the operand
    llvm::StringRef getName();

    // Returns only name for structures (and not indices) in string format and returns 
    // the same value as getName for other type of operands
    llvm::StringRef getOnlyName();
    
    // --------------------------------------------------------------------------------
};

namespace OperandRepository
{
    // Check whether a SLIMOperand object corresponds to a global or a address-taken local variable or not
    extern std::map<llvm::Value *, SLIMOperand *> value_to_slim_operand;

    // Contains the value objects that are a result of alloca instruction
    extern std::set<llvm::Value *> alloca_operand;

    // Contains the return operand of every function
    extern std::map<llvm::Function *, SLIMOperand *> function_return_operand;

    // Returns the SLIMOperand object if already exists, otherwise returns a nullptr
    SLIMOperand * getSLIMOperand(llvm::Value *value);

    // Set the SLIMOperand object corresponding to a LLVM Value object
    void setSLIMOperand(llvm::Value *value, SLIMOperand *slim_operand);

    // Returns the return operand of a function
    SLIMOperand * getFunctionReturnOperand(llvm::Function *function);

    // Sets the return operand of a function
    void setFunctionReturnOperand(llvm::Function *function, SLIMOperand *return_operand);
};