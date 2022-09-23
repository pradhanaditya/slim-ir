#include "IR.h"

long long slim::IR::total_instructions = 0;
long long slim::IR::total_basic_blocks = 0;

// Process the llvm instruction and return the corresponding SLIM instruction
BaseInstruction * slim::processLLVMInstruction(llvm::Instruction &instruction)
{
    BaseInstruction *base_instruction;
                
    if (llvm::isa<llvm::AllocaInst>(instruction))
    {
        base_instruction = new AllocaInstruction(&instruction);
    }
    else if (llvm::isa<llvm::LoadInst>(instruction))
    {
        base_instruction = new LoadInstruction(&instruction);
    }
    else if (llvm::isa<llvm::StoreInst>(instruction))
    {
        base_instruction = new StoreInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FenceInst>(instruction))
    {
        base_instruction = new FenceInstruction(&instruction);
    }
    else if (llvm::isa<llvm::AtomicCmpXchgInst>(instruction))
    {
        base_instruction = new AtomicCompareChangeInstruction(&instruction);
    }
    else if (llvm::isa<llvm::AtomicRMWInst>(instruction))
    {
        base_instruction = new AtomicModifyMemInstruction(&instruction);
    }
    else if (llvm::isa<llvm::GetElementPtrInst>(instruction))
    {
        base_instruction = new GetElementPtrInstruction(&instruction);
    }
    else if (llvm::isa<llvm::UnaryOperator>(instruction))
    {
        base_instruction = new FPNegationInstruction(&instruction);
    }
    else if (llvm::isa<llvm::BinaryOperator>(instruction))
    {
        base_instruction = new BinaryOperation(&instruction);
    }
    else if (llvm::isa<llvm::ExtractElementInst>(instruction))
    {
        base_instruction = new ExtractElementInstruction(&instruction);
    }
    else if (llvm::isa<llvm::InsertElementInst>(instruction))
    {
        base_instruction = new InsertElementInstruction(&instruction);
    }
    else if (llvm::isa<llvm::ShuffleVectorInst>(instruction))
    {
        base_instruction = new ShuffleVectorInstruction(&instruction);
    }
    else if (llvm::isa<llvm::ExtractValueInst>(instruction))
    {
        base_instruction = new ExtractValueInstruction(&instruction);
    }
    else if (llvm::isa<llvm::InsertValueInst>(instruction))
    {
        base_instruction = new InsertValueInstruction(&instruction);
    }
    else if (llvm::isa<llvm::TruncInst>(instruction))
    {
        base_instruction = new TruncInstruction(&instruction);
    }
    else if (llvm::isa<llvm::ZExtInst>(instruction))
    {
        base_instruction = new ZextInstruction(&instruction);
    }
    else if (llvm::isa<llvm::SExtInst>(instruction))
    {
        base_instruction = new SextInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FPTruncInst>(instruction))
    {
        base_instruction = new TruncInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FPExtInst>(instruction))
    {
        base_instruction = new FPExtInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FPToUIInst>(instruction))
    {
        base_instruction = new FPToIntInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FPToSIInst>(instruction))
    {
        base_instruction = new FPToIntInstruction(&instruction);
    }
    else if (llvm::isa<llvm::UIToFPInst>(instruction))
    {
        base_instruction = new IntToFPInstruction(&instruction);
    }
    else if (llvm::isa<llvm::SIToFPInst>(instruction))
    {
        base_instruction = new IntToFPInstruction(&instruction);
    }
    else if (llvm::isa<llvm::PtrToIntInst>(instruction))
    {
        base_instruction = new PtrToIntInstruction(&instruction);
    }
    else if (llvm::isa<llvm::IntToPtrInst>(instruction))
    {
        base_instruction = new IntToPtrInstruction(&instruction);
    }
    else if (llvm::isa<llvm::BitCastInst>(instruction))
    {
        base_instruction = new BitcastInstruction(&instruction);
    }
    else if (llvm::isa<llvm::AddrSpaceCastInst>(instruction))
    {
        base_instruction = new AddrSpaceInstruction(&instruction);
    }
    else if (llvm::isa<llvm::ICmpInst>(instruction))
    {
        base_instruction = new CompareInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FCmpInst>(instruction))
    {
        base_instruction = new CompareInstruction(&instruction);
    }
    else if (llvm::isa<llvm::PHINode>(instruction))
    {
        base_instruction = new PhiInstruction(&instruction);
    }
    else if (llvm::isa<llvm::SelectInst>(instruction))
    {
        base_instruction = new SelectInstruction(&instruction);
    }
    else if (llvm::isa<llvm::FreezeInst>(instruction))
    {
        base_instruction = new FreezeInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CallInst>(instruction))
    {
        base_instruction = new CallInstruction(&instruction);
    }
    else if (llvm::isa<llvm::VAArgInst>(instruction))
    {
        base_instruction = new VarArgInstruction(&instruction);
    }
    else if (llvm::isa<llvm::LandingPadInst>(instruction))
    {
        base_instruction = new LandingpadInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CatchPadInst>(instruction))
    {
        base_instruction = new CatchpadInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CleanupPadInst>(instruction))
    {
        base_instruction = new CleanuppadInstruction(&instruction);
    }
    else if (llvm::isa<llvm::ReturnInst>(instruction))
    {
        base_instruction = new ReturnInstruction(&instruction);
    }
    else if (llvm::isa<llvm::BranchInst>(instruction))
    {
        base_instruction = new BranchInstruction(&instruction);
    }
    else if (llvm::isa<llvm::SwitchInst>(instruction))
    {
        base_instruction = new SwitchInstruction(&instruction);
    }
    else if (llvm::isa<llvm::IndirectBrInst>(instruction))
    {
        base_instruction = new IndirectBranchInstruction(&instruction);
    }
    else if (llvm::isa<llvm::InvokeInst>(instruction))
    {
        base_instruction = new InvokeInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CallBrInst>(instruction))
    {
        base_instruction = new CallbrInstruction(&instruction);
    }
    else if (llvm::isa<llvm::ResumeInst>(instruction))
    {
        base_instruction = new ResumeInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CatchSwitchInst>(instruction))
    {
        base_instruction = new CatchswitchInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CatchReturnInst>(instruction))
    {
        base_instruction = new CatchreturnInstruction(&instruction);
    }
    else if (llvm::isa<llvm::CleanupReturnInst>(instruction))
    {
        base_instruction = new CleanupReturnInstruction(&instruction);
    }
    else if (llvm::isa<llvm::UnreachableInst>(instruction))
    {
        base_instruction = new UnreachableInstruction(&instruction);
    }
    else
    {
        base_instruction = new OtherInstruction(&instruction);   
    }

    return base_instruction;
}

// Default constructor
slim::IR::IR() { }

// Construct the SLIM IR from module
slim::IR::IR(std::unique_ptr<llvm::Module> &module)
{
    // Fetch the function list of the module
    llvm::SymbolTableList<llvm::Function> &function_list = module->getFunctionList();
    
    // For each function in the module
    for (llvm::Function &function : function_list)
    {    
        // Append the pointer to the function to the "functions" list
        if (!function.isIntrinsic() && !function.isDeclaration())
        {
            this->functions.push_back(&function);
        }
        else
        {
            continue ;
        }

        // For each basic block in the function
        for (llvm::BasicBlock &basic_block : function.getBasicBlockList())
        {
            // Create function-basicblock pair
            std::pair<llvm::Function *, llvm::BasicBlock *> func_basic_block{&function, &basic_block};

            this->basic_block_to_id[&basic_block] = slim::IR::total_basic_blocks;

            slim::IR::total_basic_blocks++;

            // For each instruction in the basic block 
            for (llvm::Instruction &instruction : basic_block.getInstList())
            {
                if (instruction.hasMetadataOtherThanDebugLoc() || instruction.isDebugOrPseudoInst())
                {
                    continue ;
                }
                
                BaseInstruction *base_instruction = slim::processLLVMInstruction(instruction);

                if (base_instruction->getInstructionType() == InstructionType::CALL)
                {
                    CallInstruction *call_instruction = (CallInstruction *) base_instruction;

                    if (!call_instruction->isIndirectCall() && !call_instruction->getCalleeFunction()->isDeclaration())
                    {
                        for (unsigned arg_i = 0; arg_i < call_instruction->getNumFormalArguments(); arg_i++)
                        {
                            llvm::Argument *formal_argument = call_instruction->getFormalArgument(arg_i);
                            SLIMOperand * formal_slim_argument = OperandRepository::getSLIMOperand(formal_argument);

                            if (!formal_slim_argument)
                            {
                                formal_slim_argument = new SLIMOperand(formal_argument);
                                OperandRepository::setSLIMOperand(formal_argument, formal_slim_argument);
                            }

                            LoadInstruction *new_load_instr = new LoadInstruction(&llvm::cast<llvm::CallInst>(instruction), formal_slim_argument, call_instruction->getOperand(arg_i).first);

                            // The initial value of total instructions is 0 and it is incremented after every instruction
                            long long instruction_id = slim::IR::total_instructions;

                            // Increment the total instructions count
                            slim::IR::total_instructions++;

                            base_instruction->setInstructionId(instruction_id);

                            this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                            // Map the instruction id to the corresponding SLIM instruction
                            this->inst_id_to_object[instruction_id] = new_load_instr;
                        }
                    }
                }

                // The initial value of total instructions is 0 and it is incremented after every instruction
                long long instruction_id = slim::IR::total_instructions;

                // Increment the total instructions count
                slim::IR::total_instructions++;

                base_instruction->setInstructionId(instruction_id);

                this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                // Map the instruction id to the corresponding SLIM instruction
                this->inst_id_to_object[instruction_id] = base_instruction;
            }
        }
    }
}

// Returns the total number of instructions across all the functions and basic blocks
long long slim::IR::getTotalInstructions()
{
    return slim::IR::total_instructions;
}

// Return the total number of functions in the module
unsigned slim::IR::getNumberOfFunctions()
{
    return this->functions.size();
}

// Return the total number of basic blocks in the module
long long slim::IR::getNumberOfBasicBlocks()
{
    return total_basic_blocks;
}

// Returns the pointer to llvm::Function for the function at the given index
llvm::Function * slim::IR::getLLVMFunction(unsigned index)
{
    // Make sure that the index is not out-of-bounds
    assert(index >= 0 && index < this->getNumberOfFunctions());

    return this->functions[index];
}

// Add instructions for function-basicblock pair (used by the LegacyIR)
void slim::IR::addFuncBasicBlockInstructions(llvm::Function * function, llvm::BasicBlock * basic_block)
{
    // Create function-basicblock pair
    std::pair<llvm::Function *, llvm::BasicBlock *> func_basic_block{function, basic_block};

    // For each instruction in the basic block 
    for (llvm::Instruction &instruction : basic_block->getInstList())
    {
        BaseInstruction *base_instruction = slim::processLLVMInstruction(instruction);

        // Get the instruction id
        long long instruction_id = slim::IR::total_instructions;

        // Increment the total instructions count
        slim::IR::total_instructions++;

        base_instruction->setInstructionId(instruction_id);

        if (llvm::isa<llvm::LoadInst>(instruction) || llvm::isa<llvm::StoreInst>(instruction) 
        || llvm::isa<llvm::CallInst>(instruction) || llvm::isa<llvm::GetElementPtrInst>(instruction)
        || llvm::isa<llvm::BitCastInst>(instruction) || llvm::isa<llvm::ReturnInst>(instruction)
        || llvm::isa<llvm::PHINode>(instruction))
        {

            if (llvm::isa<llvm::LoadInst>(instruction) && !(base_instruction)->hasPointerVariables()
            || llvm::isa<llvm::StoreInst>(instruction) && !(base_instruction)->hasPointerVariables())
            {
                continue ;
            }
  
            this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

            // Map the instruction id to the corresponding SLIM instruction
            this->inst_id_to_object[instruction_id] = base_instruction;
        }
    }
}

// Return the function-basicblock to instructions map (required by the LegacyIR)
std::map<std::pair<llvm::Function *, llvm::BasicBlock *>, std::list<long long>> &slim::IR::getFuncBBToInstructions()
{
    return this->func_bb_to_inst_id;
} 

// Get the instruction id to SLIM instruction map (required by the LegacyIR)
std::map<long long, BaseInstruction *> &slim::IR::getIdToInstructionsMap()
{
    return this->inst_id_to_object;
}

// Returns the first instruction id in the instruction list of the given function-basicblock pair
long long slim::IR::getFirstIns(llvm::Function* function, llvm::BasicBlock* basic_block)
{
    // Make sure that the list corresponding to the function-basicblock pair exists
    assert(this->func_bb_to_inst_id.find({function, basic_block}) != this->func_bb_to_inst_id.end());

    auto result = func_bb_to_inst_id.find({function, basic_block});
    
    return result->second.front();
}

// Returns the last instruction id in the instruction list of the given function-basicblock pair 
long long slim::IR::getLastIns(llvm::Function* function, llvm::BasicBlock* basic_block)
{
    // Make sure that the list corresponding to the function-basicblock pair exists
    assert(this->func_bb_to_inst_id.find({function, basic_block}) != this->func_bb_to_inst_id.end());

    auto result = func_bb_to_inst_id.find({function, basic_block});
    
    return result->second.back();
}

// Returns the reversed instruction list for a given function and a basic block
std::list<long long> slim::IR::getReverseInstList(llvm::Function * function, llvm::BasicBlock * basic_block)
{
    // Make sure that the list corresponding to the function-basicblock pair exists
    assert(this->func_bb_to_inst_id.find({function, basic_block}) != this->func_bb_to_inst_id.end());

    std::list<long long> inst_list = this->func_bb_to_inst_id[{function, basic_block}];

    inst_list.reverse();

    return inst_list;
}

// Returns the reversed instruction list (for the list passed as an argument)
std::list<long long> slim::IR::getReverseInstList(std::list<long long> inst_list)
{
    inst_list.reverse();
    return inst_list;
}

// Get SLIM instruction from the instruction index
BaseInstruction * slim::IR::getInstrFromIndex(long long index)
{
    return this->inst_id_to_object[index];
}

// Get basic block id
long long slim::IR::getBasicBlockId(llvm::BasicBlock *basic_block)
{
    assert(this->basic_block_to_id.find(basic_block) != this->basic_block_to_id.end());

    return this->basic_block_to_id[basic_block];
}

// Dump the IR
void slim::IR::dumpIR()
{
    // Keeps track whether the function details have been printed or not
    std::unordered_map<llvm::Function *, bool> func_visited;
    
    // Iterate over the function basic block map
    for (auto &entry : this->func_bb_to_inst_id)
    {
        llvm::Function *func = entry.first.first;
        llvm::BasicBlock *basic_block = entry.first.second;

        // Check if the function details are already printed and if not, print the details and mark
        // the function as visited
        if (func_visited.find(func) == func_visited.end())
        {
            llvm::outs() << "[" << func->getSubprogram()->getFilename() << "] " << "Function: " << func->getName() << "\n";
            llvm::outs() << "-------------------------------------" << "\n";          
            
            // Mark the function as visited
            func_visited[func] = true;
        }

        // Print the basic block name
        llvm::outs() << "Basic block " << this->getBasicBlockId(basic_block) << ": " << basic_block->getName() << " (Predecessors: ";
        llvm::outs() << "[";

        // Print the names of predecessor basic blocks
        for (auto pred = llvm::pred_begin(basic_block); pred != llvm::pred_end(basic_block); pred++)
        {
            llvm::outs() << (*pred)->getName();

            if (std::next(pred) != ((llvm::pred_end(basic_block))))
            {
                llvm::outs() << ", ";
            }
        }

        llvm::outs() << "])\n";

        for (long long instruction_id : entry.second)
        {
            BaseInstruction *instruction = inst_id_to_object[instruction_id];

            instruction->printInstruction();
        }

        llvm::outs() << "\n\n";
    }
}


void slim::IR::generateIR(std::unique_ptr<llvm::Module> &module){

    // Fetch the function list of the module
    llvm::SymbolTableList<llvm::Function> &function_list = module->getFunctionList();
    
    // For each function in the module
    for (llvm::Function &function : function_list)
    {    
        // Append the pointer to the function to the "functions" list
        if (!function.isIntrinsic() && !function.isDeclaration())
        {
            this->functions.push_back(&function);
        }
        else
        {
            continue ;
        }

        // For each basic block in the function
        for (llvm::BasicBlock &basic_block : function.getBasicBlockList())
        {
            // Create function-basicblock pair
            std::pair<llvm::Function *, llvm::BasicBlock *> func_basic_block{&function, &basic_block};

            this->basic_block_to_id[&basic_block] = slim::IR::total_basic_blocks;

            slim::IR::total_basic_blocks++;

            // For each instruction in the basic block 
            for (llvm::Instruction &instruction : basic_block.getInstList())
            {
                if (instruction.hasMetadataOtherThanDebugLoc() || instruction.isDebugOrPseudoInst())
                {
                    continue ;
                }
                
                BaseInstruction *base_instruction = slim::processLLVMInstruction(instruction);

                if (base_instruction->getInstructionType() == InstructionType::CALL)
                {
                    CallInstruction *call_instruction = (CallInstruction *) base_instruction;

                    if (!call_instruction->isIndirectCall() && !call_instruction->getCalleeFunction()->isDeclaration())
                    {
                        for (unsigned arg_i = 0; arg_i < call_instruction->getNumFormalArguments(); arg_i++)
                        {
                            llvm::Argument *formal_argument = call_instruction->getFormalArgument(arg_i);
                            SLIMOperand * formal_slim_argument = OperandRepository::getSLIMOperand(formal_argument);

                            if (!formal_slim_argument)
                            {
                                formal_slim_argument = new SLIMOperand(formal_argument);
                                OperandRepository::setSLIMOperand(formal_argument, formal_slim_argument);
                            }

                            LoadInstruction *new_load_instr = new LoadInstruction(&llvm::cast<llvm::CallInst>(instruction), formal_slim_argument, call_instruction->getOperand(arg_i).first);

                            // The initial value of total instructions is 0 and it is incremented after every instruction
                            long long instruction_id = slim::IR::total_instructions;

                            // Increment the total instructions count
                            slim::IR::total_instructions++;

                            base_instruction->setInstructionId(instruction_id);

                            this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                            // Map the instruction id to the corresponding SLIM instruction
                            this->inst_id_to_object[instruction_id] = new_load_instr;
                        }
                    }
                }

                // The initial value of total instructions is 0 and it is incremented after every instruction
                long long instruction_id = slim::IR::total_instructions;

                // Increment the total instructions count
                slim::IR::total_instructions++;

                base_instruction->setInstructionId(instruction_id);

                this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                // Map the instruction id to the corresponding SLIM instruction
                this->inst_id_to_object[instruction_id] = base_instruction;
            }
        }
    }
}


// Provides APIs similar to the older implementation of SLIM in order to support the implementations
// that are built using the older SLIM as a base 
slim::LegacyIR::LegacyIR()
{
    slim_ir = new slim::IR();
}

// Add the instructions of a basic block (of a given function) in the IR
void slim::LegacyIR::simplifyIR(llvm::Function *function, llvm::BasicBlock *basic_block)
{
    this->slim_ir->addFuncBasicBlockInstructions(function, basic_block);
}

// Get the repository (in the form of function-basicblock to instructions mappings) of all the SLIM instructions
std::map<std::pair<llvm::Function *, llvm::BasicBlock *>, std::list<long long>> &slim::LegacyIR::getfuncBBInsMap()
{
    return this->slim_ir->getFuncBBToInstructions();
}

// Get the instruction id to SLIM instruction map
std::map<long long, BaseInstruction *> &slim::LegacyIR::getGlobalInstrIndexList()
{
    return this->slim_ir->getIdToInstructionsMap();
}

// Returns the corresponding LLVM instruction for the instruction id
llvm::Instruction * slim::LegacyIR::getInstforIndx(long long index)
{
    BaseInstruction *slim_instruction = this->slim_ir->getInstrFromIndex(index);

    return slim_instruction->getLLVMInstruction();
}
