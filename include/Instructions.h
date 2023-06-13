#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Argument.h"
#include "Operand.h"
#include <vector>
#include <utility>
#include <list>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <sstream>

// Types of SLIM instructions
typedef enum
{
    ALLOCA,
    LOAD,
    STORE,
    FENCE,
    ATOMIC_COMPARE_CHANGE,
    ATOMIC_MODIFY_MEM,
    GET_ELEMENT_PTR,
    FP_NEGATION,
    BINARY_OPERATION,
    EXTRACT_ELEMENT,
    INSERT_ELEMENT,
    SHUFFLE_VECTOR,
    EXTRACT_VALUE,
    INSERT_VALUE,
    TRUNC,
    ZEXT,
    SEXT,
    FPEXT,
    FP_TO_INT,
    INT_TO_FP,
    PTR_TO_INT,
    INT_TO_PTR,
    BITCAST,
    ADDR_SPACE,
    COMPARE,
    PHI,
    SELECT,
    FREEZE,
    CALL,
    VAR_ARG,
    LANDING_PAD,
    CATCH_PAD,
    CLEANUP_PAD,
    RETURN,
    BRANCH,
    SWITCH,
    INDIRECT_BRANCH,
    INVOKE,
    CALL_BR,
    RESUME,
    CATCH_SWITCH,
    CATCH_RETURN,
    CLEANUP_RETURN,
    UNREACHABLE,
    OTHER,
    NOT_ASSIGNED
} InstructionType;

// Types of binary operations
typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    REM,
    SHIFT_LEFT,
    LOGICAL_SHIFT_RIGHT,
    ARITHMETIC_SHIFT_RIGHT,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR
} SLIMBinaryOperator;

// Types of input/scan statements (used to determine if an instruction takes an input)
typedef enum
{
    NOT_APPLICABLE,
    SCANF,
    SSCANF,
    FSCANF
} InputStatementType;

/*
    BaseInstruction class
    
    All the SLIM Instructions should inherit this class.
*/
class BaseInstruction 
{    
protected:
    // Type of the instruction
    InstructionType instruction_type;

    // Instruction id (unique for every instruction)
    long long instruction_id;

    // The corresponding LLVM instruction
    llvm::Instruction *instruction;

    // The function to which this instruction belongs
    llvm::Function *function;

    // The basic block to which this instruction belongs
    llvm::BasicBlock *basic_block;

    // The instruction is expected to be ignored in the analysis (if the flag is set)
    bool is_ignored;

    // Operands of the instruction
    std::vector<std::pair<SLIMOperand *, int>> operands;

    // Result of the instruction
    std::pair<SLIMOperand *, int> result;

    // Variants info
    std::map<unsigned, std::map<llvm::Value *, unsigned>> variants;

    // Is the instruction an input statement
    bool is_input_statement;

    // Type of the input statement
    InputStatementType input_statement_type;

    // Starting index from where the input arguments start (applicable to a valid input statement)
    unsigned starting_input_args_index;

    // Is the instruction a constant assignment
    bool is_constant_assignment;

    // Is the instruction an expression
    bool is_expression_assignment;

    // Does this SLIM instruction correspond to any statement in the original source program
    bool has_source_line_number;

    // Does this SLIM instruction involve any pointer variable (with reference to the source program)
    bool has_pointer_variables;

    // The source program line number corresponding to this instruction
    unsigned source_line_number;

public:
    // Constructor
    BaseInstruction(llvm::Instruction *instruction);

    // Sets the ID for this instruction
    void setInstructionId(long long id);

    // Returns the instruction ID
    long long getInstructionId();

    // Returns the instruction type
    InstructionType getInstructionType();

    // Returns the corresponding LLVM instruction
    llvm::Instruction * getLLVMInstruction();

    // Returns the function to which this instruction belongs
    llvm::Function * getFunction();

    // Returns the basic block to which this instruction belongs
    llvm::BasicBlock * getBasicBlock();

    // Returns true if the instruction is an input statement
    bool isInputStatement();

    // Returns the input statement type
    InputStatementType getInputStatementType();

    // Returns the starting index of the input arguments (applicable only to valid input statements)
    unsigned getStartingInputArgsIndex();
    
    // Returns true if the instruction is a constant assignment
    bool isConstantAssignment();

    // Returns true if the instruction is an expression assignment;
    bool isExpressionAssignment();
    
    // Checks whether the instruction has any relation to a statement in the source program or not
    bool hasSourceLineNumber();
    
    // Returns the source program line number corresponding to this instruction
    unsigned getSourceLineNumber();

    // Returns the source file name corresponding to this instruction (to be used only for print purposes)
    std::string getSourceFileName();

    // Sets the ignore flag
    void setIgnore();

    // Returns true if the instruction is to be ignored (during analysis)
    bool isIgnored();
    
    // Returns true if the instruction involves any pointer variable (with reference to the source program)
    bool hasPointerVariables();

    // Prints the corresponding LLVM instruction
    void printLLVMInstruction();

    // Returns the result operand
    std::pair<SLIMOperand *, int> getResultOperand();

    // Sets the result operand
    void setResultOperand(std::pair<SLIMOperand *, int> new_operand);

    // Returns the number of operands
    unsigned getNumOperands();

    // Returns the operand at a particular index
    std::pair<SLIMOperand *, int> getOperand(unsigned index);

    // Sets the operand at the given index
    void setOperand(unsigned index, std::pair<SLIMOperand *, int> new_operand);

    // Sets the indirection level of RHS operand at the given index
    void setRHSIndirection(unsigned index, unsigned new_indirection);

    // Pure virtual function - every SLIM instruction class must implement this function 
    virtual void printInstruction() = 0;

    // Insert new variant info
    void insertVariantInfo(unsigned result_ssa_version, llvm::Value *variable, unsigned variable_version);

    // Get number of variants
    unsigned getNumVariants();
    
    // Print variants
    void printMMVariants();
    
    // --------------- APIs for the Legacy SLIM --------------- //
    
    // Returns true if the instruction is a CALL instruction
    bool getCall();
    
    // Returns true if the instruction is a PHI instruction
    bool getPhi();
    
    // Return the LHS operand
    std::pair<SLIMOperand *, int> getLHS();

    // Return the RHS operand(s) list
    std::vector<std::pair<SLIMOperand *, int>> getRHS();
    // -------------------------------------------------------- //
};

// Alloca instruction
class AllocaInstruction: public BaseInstruction
{
public:
    AllocaInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Load instruction
class LoadInstruction: public BaseInstruction
{
public:
    LoadInstruction(llvm::Instruction *instruction);
    LoadInstruction(llvm::CallInst *call_instruction, SLIMOperand *result, SLIMOperand *rhs_operand);
    void printInstruction();
};

// Store instruction
class StoreInstruction: public BaseInstruction
{
public:
    StoreInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Fence instruction
class FenceInstruction: public BaseInstruction
{
public:
    FenceInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Atomic compare and change instruction
class AtomicCompareChangeInstruction: public BaseInstruction
{
protected:
    std::pair<SLIMOperand *, int> pointer_operand;
    std::pair<SLIMOperand *, int> compare_operand;
    std::pair<SLIMOperand *, int> new_value;

public:
    AtomicCompareChangeInstruction(llvm::Instruction *instruction);
    llvm::Value * getPointerOperand();
    llvm::Value * getCompareOperand();
    llvm::Value * getNewValue();
    void printInstruction();
};

// Atomic modify memory instruction
class AtomicModifyMemInstruction: public BaseInstruction
{
public:
    AtomicModifyMemInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Getelementptr instruction
class GetElementPtrInstruction: public BaseInstruction
{
    SLIMOperand * gep_main_operand;
    std::vector<SLIMOperand *> indices;

public:
    GetElementPtrInstruction(llvm::Instruction *instruction);   
    
    // Returns the main operand (corresponding to the aggregate name)
    SLIMOperand * getMainOperand();

    // Returns the number of index operands
    unsigned getNumIndexOperands();
    
    // Returns the operand corresponding to the index at the given position (0-based)
    SLIMOperand * getIndexOperand(unsigned position);

    void printInstruction();
};

// Unary operation

// Floating-point negation instruction
class FPNegationInstruction: public BaseInstruction
{
public:
    FPNegationInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Binary operations
class BinaryOperation: public BaseInstruction
{
SLIMBinaryOperator binary_operator;

public:
    BinaryOperation(llvm::Instruction *instruction);
    SLIMBinaryOperator getOperationType();
    void printInstruction();
};

// Vector operations

// Extractelement instruction
class ExtractElementInstruction: public BaseInstruction
{
public:
    ExtractElementInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Insertelement instruction
class InsertElementInstruction: public BaseInstruction
{
public:
    InsertElementInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// ShuffleVector instruction
class ShuffleVectorInstruction: public BaseInstruction
{
public:
    ShuffleVectorInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Operations for aggregates (structure and array) stored in registers

// ExtractValue instruction
class ExtractValueInstruction: public BaseInstruction
{
protected:
    std::vector<unsigned> indices;
public:
    ExtractValueInstruction(llvm::Instruction *instruction);
    unsigned getNumIndices();
    unsigned getIndex(unsigned index);
    void printInstruction();
};

// InsertValue instruction
class InsertValueInstruction: public BaseInstruction
{
public:
    InsertValueInstruction(llvm::Instruction *instruction);
    void printInstruction();    
};

// Conversion operations

// Trunc instruction
class TruncInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;

public:
    TruncInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// Zext instruction
class ZextInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;

public:
    ZextInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// Sext instruction
class SextInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;
public:
    SextInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// FPExt instruction
class FPExtInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;
public:
    FPExtInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// FPToInt instruction
class FPToIntInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;
public:
    FPToIntInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// IntToFP instruction
class IntToFPInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;
public:
    IntToFPInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// PtrToInt instruction
class PtrToIntInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;
public:
    PtrToIntInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// IntToPtr instruction
class IntToPtrInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;

public:
    IntToPtrInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// Bitcast instruction
class BitcastInstruction: public BaseInstruction
{
protected:
    llvm::Type *resulting_type;

public:
    BitcastInstruction(llvm::Instruction *instruction);
    llvm::Type * getResultingType();
    void printInstruction();
};

// AddrSpace instruction
class AddrSpaceInstruction: public BaseInstruction
{
public:
    AddrSpaceInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Other important instructions

// Compare instruction
class CompareInstruction: public BaseInstruction
{
public:
    CompareInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Phi instruction
class PhiInstruction: public BaseInstruction
{
public:
    PhiInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Select instruction
class SelectInstruction: public BaseInstruction
{
public:
    SelectInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Freeze instruction
class FreezeInstruction: public BaseInstruction
{
public:
    FreezeInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Call instruction
class CallInstruction: public BaseInstruction
{
protected:
    llvm::Function *callee_function;
    SLIMOperand *indirect_call_operand;
    bool indirect_call;
    std::vector<llvm::Argument *> formal_arguments_list;

public:
    CallInstruction(llvm::Instruction *instruction);
    bool isIndirectCall();
    SLIMOperand * getIndirectCallOperand();
    llvm::Function *getCalleeFunction();
    unsigned getNumFormalArguments();
    llvm::Argument * getFormalArgument(unsigned index);
    void printInstruction();
};

// Variable argument instruction
class VarArgInstruction: public BaseInstruction
{
public:
    VarArgInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Landingpad instruction
class LandingpadInstruction: public BaseInstruction
{
public:
    LandingpadInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Catchpad instruction
class CatchpadInstruction: public BaseInstruction
{
public:
    CatchpadInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Cleanuppad instruction
class CleanuppadInstruction: public BaseInstruction
{
public:
    CleanuppadInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Terminator instructions

// Return instruction
class ReturnInstruction: public BaseInstruction
{
protected:
    SLIMOperand *return_value;

public:
    ReturnInstruction(llvm::Instruction *instruction);
    SLIMOperand *getReturnOperand();
    llvm::Value *getReturnValue();
    void printInstruction();
};

// Branch instruction
class BranchInstruction: public BaseInstruction
{
protected:
    bool is_conditional;
public:
    BranchInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Switch instruction
class SwitchInstruction: public BaseInstruction
{
protected:
    SLIMOperand *condition_value;
    llvm::BasicBlock *default_case;
    std::vector<std::pair<llvm::ConstantInt *, llvm::BasicBlock *>> other_cases;

public:
    SwitchInstruction(llvm::Instruction *instruction);
    SLIMOperand * getConditionOperand();
    llvm::BasicBlock * getDefaultDestination();
    
    // Returns the number of cases excluding default case
    unsigned getNumberOfCases();

    llvm::ConstantInt * getConstantOfCase(unsigned case_number);
    llvm::BasicBlock * getDestinationOfCase(unsigned case_number);
    void printInstruction();
};

// Indirect branch instruction
class IndirectBranchInstruction: public BaseInstruction
{
protected:
    llvm::Value *address;
    std::vector<llvm::BasicBlock *> possible_destinations;

public:
    IndirectBranchInstruction(llvm::Instruction *instruction);
    llvm::Value *getBranchAddress();
    unsigned getNumPossibleDestinations();
    llvm::BasicBlock *getPossibleDestination(unsigned index);
    void printInstruction();
};

// Invoke instruction
class InvokeInstruction: public BaseInstruction
{
protected:
    llvm::Function *callee_function;
    SLIMOperand *indirect_call_operand;
    bool indirect_call;
    llvm::BasicBlock *normal_destination;
    llvm::BasicBlock *exception_destination;

public:
    InvokeInstruction(llvm::Instruction *instruction);
    bool isIndirectCall();
    SLIMOperand * getIndirectCallOperand();
    llvm::Function *getCalleeFunction();
    llvm::BasicBlock *getNormalDestination();
    llvm::BasicBlock *getExceptionDestination();
    void printInstruction();
};

// Callbr instruction
class CallbrInstruction: public BaseInstruction
{
protected:
    llvm::Function *callee_function;
    llvm::BasicBlock *default_destination;
    std::vector<llvm::BasicBlock *> indirect_destinations;

public:
    CallbrInstruction(llvm::Instruction *instruction);
    llvm::Function * getCalleeFunction();
    llvm::BasicBlock * getDefaultDestination();
    unsigned getNumIndirectDestinations();
    llvm::BasicBlock * getIndirectDestination(unsigned index);
    void printInstruction();
};

// Resume instruction - resumes propagation of an existing exception
class ResumeInstruction: public BaseInstruction
{
public:
    ResumeInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Catchswitch instruction
class CatchswitchInstruction: public BaseInstruction
{
public:
    CatchswitchInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Catchreturn instruction 
class CatchreturnInstruction: public BaseInstruction
{
public:
    CatchreturnInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// CleanupReturn instruction
class CleanupReturnInstruction: public BaseInstruction
{
public:
    CleanupReturnInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Unreachable instruction
class UnreachableInstruction: public BaseInstruction
{
public:
    UnreachableInstruction(llvm::Instruction *instruction);
    void printInstruction();
};

// Other instruction (not handled as of now)
class OtherInstruction: public BaseInstruction
{
public:
    OtherInstruction(llvm::Instruction *instruction);
    void printInstruction();
};