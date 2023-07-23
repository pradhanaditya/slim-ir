#ifndef IR_H
#define IR_H
#include "Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/MemorySSA.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Type.h"

namespace slim
{
// Check if the SSA variable (created using MemorySSA) already exists or not
extern std::map<std::string, bool> is_ssa_version_available;

// Process the llvm instruction and return the corresponding SLIM instruction
BaseInstruction * processLLVMInstruction(llvm::Instruction &instruction);

// Creates different SSA versions for global and address-taken local variables using Memory SSA
void createSSAVersions(std::unique_ptr<llvm::Module> &module);

// Creates the SLIM abstraction and provides APIs to interact with it
class IR 
{
protected:
    std::unique_ptr<llvm::Module> llvm_module;
    long long total_instructions;
    long long total_basic_blocks;
    long long total_call_instructions;
    long long total_direct_call_instructions;
    long long total_indirect_call_instructions;
    std::unordered_map<llvm::BasicBlock *, long long> basic_block_to_id;
    std::vector<llvm::Function *> functions;
    std::unordered_map<llvm::Function *, unsigned> num_call_instructions;

public:
    std::map<std::pair<llvm::Function *, llvm::BasicBlock *>, std::list<long long>> func_bb_to_inst_id;
    std::unordered_map<long long, BaseInstruction *> inst_id_to_object;
    
    // Default constructor
    IR();

    // Construct the SLIM IR from module
    IR(std::unique_ptr<llvm::Module> &module);

    // void generateIR(std::unique_ptr<llvm::Module> &module);
    void generateIR();

    // Returns the LLVM module
    std::unique_ptr<llvm::Module> & getLLVMModule();

    // Return the total number of instructions (across all basic blocks of all procedures)
    long long getTotalInstructions();

    // Return the total number of functions in the module
    unsigned getNumberOfFunctions();

    // Return the total number of basic blocks in the module
    long long getNumberOfBasicBlocks();

    // Returns the pointer to llvm::Function for the function at the given index
    llvm::Function * getLLVMFunction(unsigned index);

    // Add instructions for function-basicblock pair (used by the LegacyIR)
    void addFuncBasicBlockInstructions(llvm::Function * function, llvm::BasicBlock * basic_block);

    // Get the function-basicblock to instructions map (required by the LegacyIR)
    std::map<std::pair<llvm::Function *, llvm::BasicBlock *>, std::list<long long>> &getFuncBBToInstructions();

    // Get the instruction id to SLIM instruction map (required by the LegacyIR)
    std::unordered_map<long long, BaseInstruction *> &getIdToInstructionsMap();

    // Returns the first instruction id in the instruction list of the given function-basicblock pair
    long long getFirstIns(llvm::Function* function, llvm::BasicBlock* basic_block);

    // Returns the last instruction id in the instruction list of the given function-basicblock pair 
    long long getLastIns(llvm::Function* function, llvm::BasicBlock* basic_block);

    // Returns the reversed instruction list for a given function and a basic block
    std::list<long long> getReverseInstList(llvm::Function * function, llvm::BasicBlock * basic_block);

    // Returns the reversed instruction list (for the list passed as an argument)
    std::list<long long> getReverseInstList(std::list<long long> inst_list);

    // Get SLIM instruction from the instruction index
    BaseInstruction * getInstrFromIndex(long long index);

    // Get basic block id
    long long getBasicBlockId(llvm::BasicBlock *basic_block);

    // Inserts instruction at the front of the basic block (only in this abstraction)
    void insertInstrAtFront(BaseInstruction *instruction, llvm::BasicBlock *basic_block);

    // Inserts instruction at the end of the basic block (only in this abstraction)
    void insertInstrAtBack(BaseInstruction *instruction, llvm::BasicBlock *basic_block);
    
    // Optimize the IR (please use only when you are using the MemorySSAFlag)
    slim::IR * optimizeIR();

    // Dump the IR
    void dumpIR();

    unsigned getNumCallInstructions(llvm::Function *function);
};

// Provides APIs similar to the older implementation of SLIM in order to support the implementations
// that are built using the older SLIM as a base 
class LegacyIR
{
protected:
    slim::IR *slim_ir;

public:
    LegacyIR();
    void simplifyIR(llvm::Function *, llvm::BasicBlock *);
    std::map<std::pair<llvm::Function *, llvm::BasicBlock *>, std::list<long long>> &getfuncBBInsMap();

    // Get the instruction id to SLIM instruction map
    std::unordered_map<long long, BaseInstruction *> &getGlobalInstrIndexList();

    // Returns the corresponding LLVM instruction for the instruction id
    llvm::Instruction * getInstforIndx(long long index);
};
}
#endif