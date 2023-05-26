#include "IR.h"

namespace slim
{
    // Check if the SSA variable (created using MemorySSA) already exists or not
    std::map<std::string, bool> is_ssa_version_available;
}

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

// Creates different SSA versions for global and address-taken local variables using Memory SSA
void slim::createSSAVersions(std::unique_ptr<llvm::Module> &module)
{
    // Fetch the function list of module
	llvm::SymbolTableList<llvm::Function> &function_list = module->getFunctionList();

	// Contains the operand object corresponding to every global SSA variable
	std::map<std::string, llvm::Value *> ssa_variable_to_operand;

	// For each function in the module
    for (auto &function : function_list)
    {
        // Skip the function if it is intrinsic or is not defined in the translation unit
        if (function.isIntrinsic() || function.isDeclaration())
        {
            continue ;
        }

		llvm::PassBuilder PB;
        llvm::FunctionAnalysisManager *function_analysis_manager = new llvm::FunctionAnalysisManager();
        
		// Register the FunctionAnalysisManager in the pass builder
        PB.registerFunctionAnalyses(*function_analysis_manager);

		llvm::AAManager aa_manager;
		
		// Add the Basic Alias Analysis provided by LLVM
		aa_manager.registerFunctionAnalysis<llvm::BasicAA>();
		
		auto alias_analysis = aa_manager.run(function, *function_analysis_manager);

		llvm::DominatorTree *dominator_tree = &(*function_analysis_manager).getResult<llvm::DominatorTreeAnalysis>(function);

		llvm::MemorySSA *memory_ssa = new llvm::MemorySSA(function, &alias_analysis, dominator_tree);
		
        // Get the MemorySSAWalker which will be used to query about the clobbering memory definition
        llvm::MemorySSAWalker *memory_ssa_walker = memory_ssa->getWalker();

		std::map<llvm::Value *, bool> is_operand_stack_variable;

		// For each basic block in the function
        for (llvm::BasicBlock &basic_block : function.getBasicBlockList())
        {
            std::vector<llvm::Instruction *> instructions;

            for (llvm::Instruction &instruction : basic_block.getInstList())
            {
                instructions.push_back(&instruction);
            }

			// For each instruction in the basic block
            for (llvm::Instruction *instruction_ptr : instructions)
            {
                llvm::Instruction &instruction = *instruction_ptr;
                /*
					Check if the operand is a address-taken stack variable
					This assumes that the IR has been transformed by mem2reg. Since the only variables 
					that are left in "alloca" form (stack variables) after mem2reg are the variables whose 
					addresses have been taken in some form. The rest of the local variables are promoted to
					SSA registers by mem2reg. 
				*/
				if (llvm::isa<llvm::AllocaInst>(instruction))
				{
					is_operand_stack_variable[(llvm::Value *) &instruction] = true;
				}
				// Check if the instruction is a load instruction
                if (llvm::isa<llvm::LoadInst>(instruction))
                {
                    // Get the clobbering memory access for this load instruction
                    llvm::MemoryAccess *clobbering_mem_access = memory_ssa_walker->getClobberingMemoryAccess(&instruction);
	
                    std::string ssa_variable_name = "";

					// Check if this the memory access is a MemoryDef or not
                    if (llvm::isa<llvm::MemoryDef>(clobbering_mem_access) || llvm::isa<llvm::MemoryPhi>(clobbering_mem_access))
                    {
						// Cast the MemoryAccess object to MemoryDef object
                        llvm::MemoryDef *memory_def = llvm::dyn_cast<llvm::MemoryDef, llvm::MemoryAccess *>(clobbering_mem_access);
						llvm::MemoryPhi *memory_phi = llvm::dyn_cast<llvm::MemoryPhi, llvm::MemoryAccess *>(clobbering_mem_access);

						unsigned int memory_def_id;

						// Get the memory definition id
                        if (llvm::isa<llvm::MemoryDef>(clobbering_mem_access))
						{
							memory_def_id = memory_def->getID();
						}
						else
						{
							memory_def_id = memory_phi->getID();
						}

						// Fetch the source operand of the load instruction
                        llvm::Value *source_operand = instruction.getOperand(0);

						// Check if the source operand is a global variable
                        if (llvm::isa<llvm::GlobalVariable>(source_operand) || is_operand_stack_variable[source_operand])
                        {
							// Based on the memory definition id and global or address-taken local variable name, this is the expected SSA variable name
                            ssa_variable_name = function.getName().str() + "_" + source_operand->getName().str() + "_" + std::to_string(memory_def_id);

							// Check if the SSA variable (created using MemorySSA) already exists or not
                            if (slim::is_ssa_version_available.find(ssa_variable_name) != slim::is_ssa_version_available.end())
                            {
                                // If the expected SSA variable already exists, then replace the source operand with the corresponding SSA operand
							    instruction.setOperand(0, ssa_variable_to_operand[ssa_variable_name]);
                            }
                            else
                            {
								// Fetch the basic block iterator
								llvm::BasicBlock::iterator basicblock_iterator = basic_block.begin();
                                
								// Create a new load instruction which loads the value from the memory location to a temporary variable
                                llvm::LoadInst *new_load_instr = new llvm::LoadInst(source_operand->getType(), source_operand, "tmp." + ssa_variable_name, &instruction);

								// Create a new alloca instruction for the new SSA version
                                llvm::AllocaInst *new_alloca_instr = new llvm::AllocaInst(((llvm::Value *) new_load_instr)->getType() , 0, ssa_variable_name, new_load_instr);
                                
								// Create a new store instruction to store the value from the new temporary to the new SSA version of global or address-taken 
								// local variable
                                llvm::StoreInst *new_store_instr = new llvm::StoreInst((llvm::Value *) new_load_instr, (llvm::Value *) new_alloca_instr, &instruction);
                                
								// Update the map accordingly
                                slim::is_ssa_version_available[ssa_variable_name] = true;
                                
								// The value of a instruction corresponds to the result of that instruction 
								ssa_variable_to_operand[ssa_variable_name] = (llvm::Value *) new_alloca_instr; 

								// Replace the operand of the load instruction with the new SSA version
                                instruction.setOperand(0, ssa_variable_to_operand[ssa_variable_name]);
                            }
                        }
                    }
                    else
                    {
						// This is not expected
                        llvm_unreachable("Clobbering access is not MemoryDef, which is unexpected!");
                    }
				}
            }
        }
    }
}

// Default constructor
slim::IR::IR() 
{ 
    this->total_basic_blocks = 0;
    this->total_instructions = 0;
}

// Construct the SLIM IR from module
slim::IR::IR(std::unique_ptr<llvm::Module> &module)
{
    this->llvm_module = std::move(module);
    this->total_basic_blocks = 0;
    this->total_instructions = 0;

    // Create different SSA versions for globals and address-taken local variables if the MemorySSA flag is passed
    #ifdef MemorySSAFlag
    slim::createSSAVersions(this->llvm_module);
    #endif

    // Fetch the function list of the module
    llvm::SymbolTableList<llvm::Function> &function_list = llvm_module->getFunctionList();

    // Keeps track of the temporaries who are already renamed
    std::set<llvm::Value *> renamed_temporaries;

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

        #ifdef DiscardPointers
        std::set<llvm::Value *> discarded_result_operands;
        #endif

        // For each basic block in the function
        for (llvm::BasicBlock &basic_block : function.getBasicBlockList())
        {
            // Create function-basicblock pair
            std::pair<llvm::Function *, llvm::BasicBlock *> func_basic_block{&function, &basic_block};

            this->basic_block_to_id[&basic_block] = this->total_basic_blocks;

            this->total_basic_blocks++;

            // For each instruction in the basic block 
            for (llvm::Instruction &instruction : basic_block.getInstList())
            {
                if (instruction.isDebugOrPseudoInst())
                {
                    continue ;
                }
                
                // Ensure that all temporaries have unique name (globally) by appending the function name 
                // after the temporary name
                for (unsigned i = 0; i < instruction.getNumOperands(); i++)
                {
                    llvm::Value *operand_i = instruction.getOperand(i);

                    if (llvm::isa<llvm::GlobalValue>(operand_i)) continue ;

                    if (operand_i->hasName() && renamed_temporaries.find(operand_i) == renamed_temporaries.end())
                    {
                        llvm::StringRef old_name = operand_i->getName();
                        operand_i->setName(old_name + "_" + function.getName());
                        renamed_temporaries.insert(operand_i);
                    }
                }
                
                BaseInstruction *base_instruction = slim::processLLVMInstruction(instruction);

                #ifdef DiscardPointers
                    bool is_discarded = false;

                    for (unsigned i = 0; i < base_instruction->getNumOperands(); i++)
                    {
                        SLIMOperand *operand_i = base_instruction->getOperand(i).first;

                        if (!operand_i || !operand_i->getValue()) continue ;
                        if (operand_i->isPointerInLLVM() || (operand_i->getValue() && discarded_result_operands.find(operand_i->getValue()) != discarded_result_operands.end()))
                        {
                            is_discarded = true;
                            break;
                        }
                    }

                    if (is_discarded && base_instruction->getResultOperand().first && base_instruction->getResultOperand().first->getValue())
                    {
                        discarded_result_operands.insert(base_instruction->getResultOperand().first->getValue());
                        continue ;
                    }
                    else if (is_discarded)
                    {
                        // Ignore the instruction (because it is using the discarded value)
                        continue ;
                    }
                #endif

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

                                if (formal_argument->hasName() && renamed_temporaries.find(formal_argument) == renamed_temporaries.end())
                                {
                                    llvm::StringRef old_name = formal_argument->getName();
                                    formal_argument->setName(old_name + "_" + call_instruction->getCalleeFunction()->getName());
                                    renamed_temporaries.insert(formal_argument);
                                }

                                formal_slim_argument->setFormalArgument();
                            }

                            LoadInstruction *new_load_instr = new LoadInstruction(&llvm::cast<llvm::CallInst>(instruction), formal_slim_argument, call_instruction->getOperand(arg_i).first);

                            // The initial value of total instructions is 0 and it is incremented after every instruction
                            long long instruction_id = slim::IR::total_instructions;

                            // Increment the total instructions count
                            this->total_instructions++;

                            new_load_instr->setInstructionId(instruction_id);

                            this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                            // Map the instruction id to the corresponding SLIM instruction
                            this->inst_id_to_object[instruction_id] = new_load_instr;
                        }
                    }
                }

                // The initial value of total instructions is 0 and it is incremented after every instruction
                long long instruction_id = this->total_instructions;

                // Increment the total instructions count
                this->total_instructions++;

                base_instruction->setInstructionId(instruction_id);

                this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                // Map the instruction id to the corresponding SLIM instruction
                this->inst_id_to_object[instruction_id] = base_instruction;

                // Check if the instruction is a "Return" instruction
                if (base_instruction->getInstructionType() == InstructionType::RETURN)
                {
                    // As we are using the 'mergereturn' pass, there is only one return statement in every function
                    // and therefore, we will have only 1 return operand which we store in the function_return_operand
                    // map
                    ReturnInstruction *return_instruction = (ReturnInstruction *) base_instruction;

                    if (return_instruction->getNumOperands() == 0)
                    {
                        OperandRepository::setFunctionReturnOperand(&function, nullptr);
                    }
                    else
                    {
                        OperandRepository::setFunctionReturnOperand(&function, return_instruction->getReturnOperand());
                    }
                }
            }
        }
    }

    llvm::outs() << "Total number of functions: " << functions.size() << "\n";
    llvm::outs() << "Total number of basic blocks: " << total_basic_blocks << "\n";
    llvm::outs() << "Total number of instructions: " << total_instructions << "\n";
}

// Returns the total number of instructions across all the functions and basic blocks
long long slim::IR::getTotalInstructions()
{
    return this->total_instructions;
}

// Return the total number of functions in the module
unsigned slim::IR::getNumberOfFunctions()
{
    return this->functions.size();
}

// Return the total number of basic blocks in the module
long long slim::IR::getNumberOfBasicBlocks()
{
    return this->total_basic_blocks;
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
        long long instruction_id = this->total_instructions;

        // Increment the total instructions count
        this->total_instructions++;

        base_instruction->setInstructionId(instruction_id);

        this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

        // Map the instruction id to the corresponding SLIM instruction
        this->inst_id_to_object[instruction_id] = base_instruction;
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

    #ifndef DISABLE_IGNORE_EFFECT
    auto it = result->second.begin();

    while (it != result->second.end() && this->inst_id_to_object[*it]->isIgnored())
    {
        it++;
    }

    return (it == result->second.end() ? -1 : (*it));
    #else
    return result->second.front();
    #endif
}

// Returns the last instruction id in the instruction list of the given function-basicblock pair 
long long slim::IR::getLastIns(llvm::Function* function, llvm::BasicBlock* basic_block)
{
    // Make sure that the list corresponding to the function-basicblock pair exists
    assert(this->func_bb_to_inst_id.find({function, basic_block}) != this->func_bb_to_inst_id.end());

    auto result = func_bb_to_inst_id.find({function, basic_block});
    
    #ifndef DISABLE_IGNORE_EFFECT
    auto it = result->second.rbegin();

    while (it != result->second.rend() && this->inst_id_to_object[*it]->isIgnored())
    {
        it++;
    }

    return (it == result->second.rend() ? -1 : (*it));
    #else
    return result->second.back();
    #endif
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

// Optimize the IR (please use only when you are using the MemorySSAFlag)
slim::IR * slim::IR::optimizeIR()
{
    // Create the new slim::IR object which would contain the IR instructions after optimization
    slim::IR *optimized_slim_ir = new slim::IR();
    
	//errs() << "funcBBInsMap size: " << funcBBInsMap.size() << "\n";
	
	// Now, we are ready to do the load-store optimization
	for (auto func_basicblock_instr_entry : this->func_bb_to_inst_id)
	{
        // Add the function-basic-block entry in optimized_slim_ir
        optimized_slim_ir->func_bb_to_inst_id[func_basicblock_instr_entry.first] = std::list<long long>{};

		std::list<long long> &instruction_list = func_basicblock_instr_entry.second;
 
		//errs() << "instruction_list size: " << instruction_list.size() << "\n";
    
        long long temp_instr_counter = 0;
		std::map<llvm::Value *, BaseInstruction *> temp_token_to_instruction;
        std::map<long long, BaseInstruction *> temp_instructions;
        std::map<BaseInstruction *, long long> temp_instruction_ids;

		for (auto instruction_id : instruction_list)
		{
			//errs() << "Instruction id : " << instruction_id << "\n";
            BaseInstruction *slim_instruction = this->inst_id_to_object[instruction_id];
            
			// Check if the corresponding LLVM instruction is a Store Instruction
			if (slim_instruction->getInstructionType() == InstructionType::STORE && slim_instruction->getNumOperands() == 1 && slim_instruction->getResultOperand().first != nullptr)
			{
				// Retrieve the RHS operand of the SLIM instruction (corresponding to the LLVM IR store instruction)
				std::pair<SLIMOperand *, int> slim_instr_rhs = slim_instruction->getOperand(0);

				// Extract the value and its indirection level from slim_instr_rhs			
				llvm::Value *slim_instr_rhs_value = slim_instr_rhs.first->getValue();
				int token_indirection = slim_instr_rhs.second;

				// Check if the RHS Value is defined in an earlier SLIM statement	
				if (temp_token_to_instruction.find(slim_instr_rhs_value) != temp_token_to_instruction.end())
				{
					// Get the instruction (in the unoptimized SLIM IR)
					BaseInstruction * value_def_instr = temp_token_to_instruction[slim_instr_rhs_value];
                    long long value_def_index = temp_instruction_ids[value_def_instr];

					// Check if the statement is a load instruction
					bool is_load_instr = (llvm::isa<llvm::LoadInst>(value_def_instr->getLLVMInstruction()));

					// Get the indirection level of the LHS operand in the load instruction
					int map_entry_indirection = value_def_instr->getResultOperand().second;
				
					// Get the indirection level of the RHS operand in the load instruction
					int map_entry_rhs_indirection = value_def_instr->getOperand(0).second;

					// Adjust the indirection level
					int distance = token_indirection - map_entry_indirection + map_entry_rhs_indirection;

					// Check if the RHS is a SSA variable (created using MemorySSA)
					bool is_rhs_global_ssa_variable = (slim::is_ssa_version_available.find(slim_instr_rhs_value->getName().str()) != slim::is_ssa_version_available.end());

					// Modify the RHS operand with the new indirection level if it does not exceed 2
					if (is_load_instr && (distance >= 0 && distance <= 2) && !is_rhs_global_ssa_variable)
					{
						//errs() << slim_instr_rhs.first->getName() << " = name\n";
						
						// Set the indirection level of the RHS operand to the adjusted indirection level
						value_def_instr->setRHSIndirection(0, distance);

						// Update the RHS operand of the store instruction
						slim_instruction->setOperand(0, value_def_instr->getOperand(0));

						// Remove the existing entries
						temp_token_to_instruction.erase(slim_instr_rhs_value);
                        temp_instructions.erase(value_def_index);
                    }
				}	
				else
				{
					//errs() << "RHS is not present as LHS!\n";
				}

				// Check whether the LHS operand can be replaced
                std::pair<SLIMOperand *, int> slim_instr_lhs = slim_instruction->getResultOperand();

				// Extract the value and its indirection level from slim_instr_rhs			
				llvm::Value *slim_instr_lhs_value = slim_instr_lhs.first->getValue();
				token_indirection = slim_instr_lhs.second;

				// Check if the LHS Value is defined in an earlier SLIM statement	
				if (temp_token_to_instruction.find(slim_instr_lhs_value) != temp_token_to_instruction.end())
				{
					// Get the instruction (in the unoptimized SLIM IR)
					BaseInstruction * value_def_instr = temp_token_to_instruction[slim_instr_lhs_value];
					long long value_def_index = temp_instruction_ids[value_def_instr];

					// Check if the statement is a load instruction
					bool is_load_instr = (llvm::isa<llvm::LoadInst>(value_def_instr->getLLVMInstruction()));

					// Get the indirection level of the LHS operand in the load instruction
					int map_entry_indirection = value_def_instr->getResultOperand().second;
				
					// Get the indirection level of the RHS operand in the load instruction
					int map_entry_rhs_indirection = value_def_instr->getOperand(0).second;

					// Adjust the indirection level
					int distance = token_indirection - map_entry_indirection + map_entry_rhs_indirection;

					// Check if the RHS is a SSA variable (created using MemorySSA)
					bool is_rhs_global_ssa_variable = (slim::is_ssa_version_available.find(slim_instr_lhs_value->getName().str()) != slim::is_ssa_version_available.end());

					// Modify the RHS operand with the new indirection level if it does not exceed 2
					if (is_load_instr && (distance >= 0 && distance <= 2) && !is_rhs_global_ssa_variable)
					{
						//errs() << slim_instr_rhs.first->getName() << " = name\n";
						
						// Set the indirection level of the RHS operand to the adjusted indirection level
						value_def_instr->setRHSIndirection(0, distance);

						// Update the result operand of the store instruction
						slim_instruction->setResultOperand(value_def_instr->getOperand(0));

						// Remove the existing entries
						temp_token_to_instruction.erase(slim_instr_lhs_value);
                        temp_instructions.erase(value_def_index);
                    }
				}

				// Add the SLIM instruction (whether modified or not) 
                if (slim_instruction->getInstructionType() == InstructionType::LOAD)
				    temp_token_to_instruction[slim_instruction->getResultOperand().first->getValue()] = slim_instruction;
				temp_instructions[temp_instr_counter] = slim_instruction;
                temp_instruction_ids[slim_instruction] = temp_instr_counter;
                temp_instr_counter++;
			}
			else
			{
				//errs() << "Size != 1\n";
				// Add the SLIM instruction
                // Add the SLIM instruction (whether modified or not) 
				if (slim_instruction->getInstructionType() == InstructionType::LOAD)
				    temp_token_to_instruction[slim_instruction->getResultOperand().first->getValue()] = slim_instruction;
				temp_instructions[temp_instr_counter] = slim_instruction;
                temp_instruction_ids[slim_instruction] = temp_instr_counter;
                temp_instr_counter++;
			}
		}

		// Now, we have the final list of optimized instructions in this basic block. So, 
		// we insert the instructions in the optimized global instructions list and the 
		// instruction indices (after the optimization) in the func_basic_block_optimized_instrs
		// map
		for (auto temp_instruction : temp_instructions)
		{
            temp_instruction.second->setInstructionId(optimized_slim_ir->total_instructions);
            optimized_slim_ir->func_bb_to_inst_id[func_basicblock_instr_entry.first].push_back(optimized_slim_ir->total_instructions);
            optimized_slim_ir->inst_id_to_object[optimized_slim_ir->total_instructions] = temp_instruction.second;
            optimized_slim_ir->total_instructions++;
		}

        optimized_slim_ir->basic_block_to_id[func_basicblock_instr_entry.first.second] = optimized_slim_ir->total_basic_blocks++;
	}

    return optimized_slim_ir;
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
            if (func->getSubprogram())
                llvm::outs() << "[" << func->getSubprogram()->getFilename() << "] ";
            
            llvm::outs() << "Function: " << func->getName() << "\n";
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


void slim::IR::generateIR(){

    // Fetch the function list of the module
    llvm::SymbolTableList<llvm::Function> &function_list = this->llvm_module->getFunctionList();
    
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

            this->basic_block_to_id[&basic_block] = this->total_basic_blocks;

            this->total_basic_blocks++;

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
                            long long instruction_id = this->total_instructions;

                            // Increment the total instructions count
                            this->total_instructions++;

                            new_load_instr->setInstructionId(instruction_id);

                            this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                            // Map the instruction id to the corresponding SLIM instruction
                            this->inst_id_to_object[instruction_id] = new_load_instr;
                        }
                    }
                }

                // The initial value of total instructions is 0 and it is incremented after every instruction
                long long instruction_id = this->total_instructions;

                // Increment the total instructions count
                this->total_instructions++;

                base_instruction->setInstructionId(instruction_id);

                this->func_bb_to_inst_id[func_basic_block].push_back(instruction_id);

                // Map the instruction id to the corresponding SLIM instruction
                this->inst_id_to_object[instruction_id] = base_instruction;
            }
        }
    }
}

// Returns the LLVM module
std::unique_ptr<llvm::Module> & slim::IR::getLLVMModule()
{
    return this->llvm_module;
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
