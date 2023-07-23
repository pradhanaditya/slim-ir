#include "Instructions.h"

// Base instruction
BaseInstruction::BaseInstruction(llvm::Instruction *instruction)
{
    this->instruction = instruction;
    this->instruction_type = NOT_ASSIGNED;
    this->has_pointer_variables = false;
    this->has_source_line_number = false;
    this->basic_block = instruction->getParent();
    this->function = this->basic_block->getParent();
    this->is_constant_assignment = false;
    this->is_expression_assignment = false;
    this->is_input_statement = false;    
    this->is_ignored = false;
    this->input_statement_type = NOT_APPLICABLE;

    // Set the source line number
    if (instruction->getDebugLoc())
    {
        this->source_line_number = instruction->getDebugLoc().getLine();
        has_source_line_number = true;
    }
}

void BaseInstruction::setInstructionId(long long id)
{
    // The id should be greater than or equal to 0
    assert(id >= 0);

    this->instruction_id = id;
}

long long BaseInstruction::getInstructionId()
{
    // The instruction id should be a non-negative integer
    assert(this->instruction_id >= 0);

    return this->instruction_id;
}

// Returns the instruction type
InstructionType BaseInstruction::getInstructionType()
{
    // Make sure that the instruction has been assigned a type during its construction
    assert(this->instruction_type != InstructionType::NOT_ASSIGNED);

    return this->instruction_type;
}

// Returns the corresponding LLVM instruction
llvm::Instruction * BaseInstruction::getLLVMInstruction()
{
    return this->instruction;
}

// Returns the function to which this instruction belongs
llvm::Function * BaseInstruction::getFunction()
{
    // The function should not be NULL
    assert(this->function != nullptr);

    return this->function;
}

// Returns the basic block to which this instruction belongs
llvm::BasicBlock * BaseInstruction::getBasicBlock()
{
    // The basic block should not be NULL
    assert(this->basic_block != nullptr);

    return this->basic_block;
}

// Returns true if the instruction is an input statement
bool BaseInstruction::isInputStatement()
{
    return this->is_input_statement;
}

// Returns the input statement type
InputStatementType BaseInstruction::getInputStatementType()
{
    return this->input_statement_type;
}

// Returns the starting index of the input arguments (applicable only to valid input statements)
unsigned BaseInstruction::getStartingInputArgsIndex()
{
    assert(this->input_statement_type != InputStatementType::NOT_APPLICABLE);

    return this->starting_input_args_index;
}

// Returns true if the instruction is a constant assignment
bool BaseInstruction::isConstantAssignment()
{
    return this->is_constant_assignment;
}

// Returns true if the instruction is an expression assignment;
bool BaseInstruction::isExpressionAssignment()
{
    return this->is_expression_assignment;
}

// Checks whether the instruction has any relation to a statement in the source program or not
bool BaseInstruction::hasSourceLineNumber()
{
    return this->has_source_line_number;
}

// Returns the source program line number corresponding to this instruction
unsigned BaseInstruction::getSourceLineNumber()
{
    // Check whether the instruction corresponds to a statement in the source program or not
    assert(has_source_line_number);

    return this->source_line_number;
}

// Returns the source file name corresponding to this instruction (to be used only for print purposes)
std::string BaseInstruction::getSourceFileName()
{
    // Check whether the instruction corresponds to a statement in the source program or not
    assert(has_source_line_number);

    return this->instruction->getDebugLoc().get()->getFilename().str();
}

// Sets the ignore flag
void BaseInstruction::setIgnore()
{
    this->is_ignored = true;
}

// Returns true if the instruction is to be ignored (during analysis)
bool BaseInstruction::isIgnored()
{
    return this->is_ignored;
}

// Returns whether the instruction has pointer variables or not
bool BaseInstruction::hasPointerVariables()
{
    return this->has_pointer_variables;
}

// Returns the result operand
std::pair<SLIMOperand *, int> BaseInstruction::getResultOperand()
{
    return this->result;
}

// Sets the result operand
void BaseInstruction::setResultOperand(std::pair<SLIMOperand *, int> new_operand)
{
    this->result = new_operand;
}

// Returns the number of operands
unsigned BaseInstruction::getNumOperands()
{
    return this->operands.size();
}

// Sets the operand at the given index
void BaseInstruction::setOperand(unsigned index, std::pair<SLIMOperand *, int> new_operand)
{
    // Check if the index is less than the total number of RHS operands of this instruction
    assert(index < this->getNumOperands());

    this->operands[index] = new_operand;
}

// Sets the indirection level of RHS operand at the given index
void BaseInstruction::setRHSIndirection(unsigned index, unsigned new_indirection)
{
    // Check if the index is less than the total number of RHS operands of this instruction
    assert(index < this->getNumOperands());

    // Update the indirection
    this->operands[index].second = new_indirection;
}

// Create and return a variant of this instruction
/*BaseInstruction * BaseInstruction::createVariant(std::pair<SLIMOperand *, int> result, std::vector<std::pair<SLIMOperand *, int>> operands)
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
    else
    {
        llvm_unreachable("[Error] Variant cannot be created for this instruction!");
    }
}
*/

// Returns the operand at a particular index
std::pair<SLIMOperand *, int> BaseInstruction::getOperand(unsigned index)
{
    // The index should not be out-of-bounds
    assert(index >= 0 && index < this->getNumOperands());

    return this->operands[index];
}

// Prints the corresponding LLVM instruction
void BaseInstruction::printLLVMInstruction()
{
    this->instruction->print(llvm::outs());

    llvm::outs() << "\n";
}

void BaseInstruction::insertVariantInfo(unsigned result_ssa_version, llvm::Value *variable, unsigned variable_version)
{
    llvm::outs() << "result_ssa_version_insert = " << result_ssa_version << "\n";
    
    this->variants[result_ssa_version][variable] = variable_version;
    llvm::outs() << "Inserted variant for...\n";
    this->printLLVMInstruction();
}

// Get number of variants
unsigned BaseInstruction::getNumVariants()
{
    return this->variants.size();
}

// Print variants
void BaseInstruction::printMMVariants()
{
    if (this->variants.empty())
    {
        llvm::outs() << "No variants for this instruction...\n";
        this->printInstruction();
        return ;
    }

    llvm::outs() << "Number of variants : " << this->variants.size() << "\n";
    
    for (auto variant : this->variants)
    {
        if (this->getResultOperand().first)
        {
            unsigned result_ssa_version = variant.first;

            //llvm::outs() << "Result SSA Version: " << result_ssa_version << "\n";

            this->getResultOperand().first->setSSAVersion(result_ssa_version);

            for (unsigned i = 0; i < this->getNumOperands(); i++)
            {
                SLIMOperand * slim_operand_i = this->getOperand(i).first;

                llvm::Value * variable = slim_operand_i->getValue();

                slim_operand_i->setSSAVersion(variant.second[variable]);
            }

            this->printInstruction();

            for (unsigned i = 0; i < this->getNumOperands(); i++)
            {
                SLIMOperand * slim_operand_i = this->getOperand(i).first;

                slim_operand_i->resetSSAVersion();
            }
        }
        else
        {
            this->printInstruction();
            break;
        }
    }
}

// --------------- APIs for the Legacy SLIM ---------------
    
// Returns true if the instruction is a call instruction
bool BaseInstruction::getCall()
{
    return (this->getInstructionType() == InstructionType::CALL);
}
    
// Returns true if the instruction is a phi instruction
bool BaseInstruction::getPhi()
{
    return (this->getInstructionType() == InstructionType::PHI);    
}

// Return the LHS operand
std::pair<SLIMOperand *, int> BaseInstruction::getLHS()
{
    return this->result;
}

// Return the RHS operand(s) list
std::vector<std::pair<SLIMOperand *, int>> BaseInstruction::getRHS()
{
    return this->operands;
}

// --------------------------------------------------------

// Alloca instruction (this instruction does not contain any RHS operands)
AllocaInstruction::AllocaInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to ALLOCA
    this->instruction_type = InstructionType::ALLOCA;
    
    SLIMOperand *new_operand = new SLIMOperand((llvm::Value *) instruction, true);
    this->result = std::make_pair(new_operand, 1);
    OperandRepository::alloca_operand.insert(this->result.first->getValue());
    OperandRepository::setSLIMOperand((llvm::Value *) instruction, this->result.first);
}

void AllocaInstruction::printInstruction() { }

// Load instruction (transformed like an assignment statement)
LoadInstruction::LoadInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to LOAD
    this->instruction_type = InstructionType::LOAD;

    this->is_expression_assignment = true;

    // The value of an instruction is the result operand
    llvm::Value *result_operand = (llvm::Value *) this->instruction;

    // Create the SLIM operand corresponding to the result operand
    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand, false);

    this->result = std::make_pair(result_slim_operand, 1);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);
    //OperandRepository::setSLIMOperand((Value *) instruction, this->result);

    llvm::Value *rhs_operand = this->instruction->getOperand(0);

    SLIMOperand *rhs_slim_operand = OperandRepository::getSLIMOperand(rhs_operand);

    if (!rhs_slim_operand)
    {
        // This means the variable is surely not an address-taken local variable (otherwise it would
        // be already present in the map because of alloca instruction)
        if (llvm::isa<llvm::GlobalValue>(rhs_operand))
        {
            rhs_slim_operand = new SLIMOperand(rhs_operand, true);
        }
        else
        {
            rhs_slim_operand = new SLIMOperand(rhs_operand);
        }

        OperandRepository::setSLIMOperand(rhs_operand, rhs_slim_operand);
    }

    if (rhs_slim_operand->isGlobalOrAddressTaken() || rhs_slim_operand->isGEPInInstr())
    {
        this->operands.push_back(std::make_pair(rhs_slim_operand, 1));
    }
    else
    {
        this->operands.push_back(std::make_pair(rhs_slim_operand, 2));
    }

    if (result_slim_operand->getValue()->getType()->isPointerTy())
    {
        this->has_pointer_variables = true;
    }
}

// Used for creating assignment statements of the formal-to-actual arguments (of a Call instruction)
LoadInstruction::LoadInstruction(llvm::CallInst *call_instruction, SLIMOperand *result, SLIMOperand *rhs_operand): BaseInstruction(call_instruction)
{
    // Set the instruction type to LOAD
    this->instruction_type = InstructionType::LOAD;

    this->is_expression_assignment = true;

    this->result = std::make_pair(result, 1);

    if (rhs_operand && (rhs_operand->getValue() != nullptr))
    {
        llvm::Value * rhs_operand_after_strip = llvm::dyn_cast<llvm::Value>(rhs_operand->getValue()->stripPointerCasts());

        if (rhs_operand_after_strip)
        {
            rhs_operand = new SLIMOperand(rhs_operand_after_strip);
        }
    }
    
    // this->operands.push_back(std::make_pair(rhs_operand, 1));
    
    if (rhs_operand->isPointerVariable())
    {
        this->has_pointer_variables = true;
    }

    if (rhs_operand->isGlobalOrAddressTaken() || rhs_operand->isGEPInInstr() || llvm::isa<llvm::GlobalValue>(rhs_operand->getValue()))
    {
        this->operands.push_back(std::make_pair(rhs_operand, 0));
    }
    else 
    {
        this->operands.push_back(std::make_pair(rhs_operand, 1));
    }
}

// Prints the load instruction
void LoadInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << "<";
    
    this->result.first->printOperand(llvm::outs());
    
    llvm::outs() << ", " << this->result.second << "> = ";

    llvm::outs() << "<";
    
    this->operands[0].first->printOperand(llvm::outs());
    
    llvm::outs() << ", " << this->operands[0].second << ">\n";
}

// Store instruction
StoreInstruction::StoreInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to STORE
    this->instruction_type = InstructionType::STORE;

    // Get the result operand (operand corresponding to where the value is stored)
    llvm::Value *result_operand = this->instruction->getOperand(1);
    SLIMOperand *result_slim_operand = OperandRepository::getSLIMOperand(result_operand);
    bool is_result_gep = llvm::isa<llvm::GEPOperator>(result_operand);
    
    if (!result_slim_operand)
    {
        // The variable is not an address-taken local variable (otherwise it would
        // be already present in the map because of alloca instruction)
        if (llvm::isa<llvm::GEPOperator>(result_operand))
        {
            llvm::Value *gep_operand = llvm::cast<llvm::GEPOperator>(result_operand)->getOperand(0);

            //result_slim_operand = OperandRepository::getSLIMOperand(gep_operand);

            // if (!result_slim_operand)
            // {
            //     if (llvm::isa<llvm::GlobalValue>(gep_operand))
            //     {
            //         result_slim_operand = new SLIMOperand(gep_operand, true);
            //     }
            //     else
            //     {
            //         result_slim_operand = new SLIMOperand(gep_operand);
            //     }
            // }

            if (llvm::isa<llvm::GlobalValue>(gep_operand))
            {
                result_slim_operand = new SLIMOperand(result_operand, true);
            }
            else
            {
                result_slim_operand = new SLIMOperand(result_operand);
            }

            result_slim_operand->setIsPointerVariable();
        }
        else if (llvm::isa<llvm::GlobalValue>(result_operand))
        {
            result_slim_operand = new SLIMOperand(result_operand, true);
        }
        else if (!llvm::isa<llvm::GlobalVariable>(result_operand) && llvm::isa<llvm::Constant>(result_operand))
        {
            result_slim_operand = new SLIMOperand(result_operand, false);
        }
        else if (llvm::isa<llvm::GlobalVariable>(result_operand))
        {
            result_slim_operand = new SLIMOperand(result_operand, true);
        }
        else
        {
            result_slim_operand = new SLIMOperand(result_operand);
        }

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);
    }
    
    // Operand can be either a constant, an address-taken local variable, a function argument, 
    // a global variable or a temporary variable
    llvm::Value *rhs_operand = this->instruction->getOperand(0);
    bool is_rhs_gep = llvm::isa<llvm::GEPOperator>(rhs_operand);
    SLIMOperand *rhs_slim_operand = OperandRepository::getSLIMOperand(rhs_operand);

    if (!rhs_slim_operand)
    {
        // This means the variable is surely not an address-taken local variable (otherwise it would
        // be already present in the map because of alloca instruction)
        if (!llvm::isa<llvm::GlobalVariable>(rhs_operand) && llvm::isa<llvm::Constant>(rhs_operand))
        {
            rhs_slim_operand = new SLIMOperand(rhs_operand, false);
        }
        else if (llvm::isa<llvm::GlobalVariable>(rhs_operand))
        {
            rhs_slim_operand = new SLIMOperand(rhs_operand, true);
        }
        else
        {
            rhs_slim_operand = new SLIMOperand(rhs_operand, false);
        }

        OperandRepository::setSLIMOperand(rhs_operand, rhs_slim_operand);
    }

    if (llvm::isa<llvm::Constant>(rhs_operand) && !rhs_operand->hasName() && !rhs_slim_operand->isGEPInInstr())
    {
        this->is_constant_assignment = true;

        if (llvm::isa<llvm::GlobalValue>(result_operand) || result_slim_operand->isGEPInInstr() || result_slim_operand->isAlloca())
        {
            this->result = std::make_pair(result_slim_operand, 1);
        }
        else
        {
            this->result = std::make_pair(result_slim_operand, 2);
        }
        
        // 0 represents that the operand is a constant and not a memory location
        this->operands.push_back(std::make_pair(rhs_slim_operand, 0));        
    }
    else
    {
        this->is_expression_assignment = true;

        if ((result_slim_operand->isGlobalOrAddressTaken() || result_slim_operand->isGEPInInstr()) && (rhs_slim_operand->isGlobalOrAddressTaken() || rhs_slim_operand->isGEPInInstr()))
        {
            this->result = std::make_pair(result_slim_operand, 1);
            this->operands.push_back(std::make_pair(rhs_slim_operand, 0));
        }
        else if ((result_slim_operand->isGlobalOrAddressTaken() || result_slim_operand->isGEPInInstr()) && (!(rhs_slim_operand->isGlobalOrAddressTaken() || rhs_slim_operand->isGEPInInstr())))
        {
            this->result = std::make_pair(result_slim_operand, 1);
            this->operands.push_back(std::make_pair(rhs_slim_operand, 1));
        }
        else if ((!(result_slim_operand->isGlobalOrAddressTaken() || result_slim_operand->isGEPInInstr())) && (rhs_slim_operand->isGlobalOrAddressTaken() || rhs_slim_operand->isGEPInInstr()))
        {
            this->result = std::make_pair(result_slim_operand, 2);
            this->operands.push_back(std::make_pair(rhs_slim_operand, 0));
        }
        else if ((!(result_slim_operand->isGlobalOrAddressTaken() || result_slim_operand->isGEPInInstr())) && (!(rhs_slim_operand->isGlobalOrAddressTaken() || rhs_slim_operand->isGEPInInstr())))
        {
            this->result = std::make_pair(result_slim_operand, 2);
            this->operands.push_back(std::make_pair(rhs_slim_operand, 1));
        }
        else
        {
            // It should not be possible to reach this block of code
            llvm_unreachable("[StoreInstruction Error] Unexpected behaviour!\n");
        }
    }


    if (llvm::isa<llvm::ConstantData>(rhs_operand) && !rhs_operand->getType()->isPointerTy())
    {
        this->has_pointer_variables = false;
        result_slim_operand->unsetIsPointerVariable();
    }
    else if (result_slim_operand->isPointerVariable() || rhs_slim_operand->isPointerVariable())
    {
        this->has_pointer_variables = true;
    }
}

void StoreInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << "<";

    this->result.first->printOperand(llvm::outs()); 
    
    llvm::outs() << ", " << this->result.second << "> = ";

    llvm::outs() << "<";

    this->operands[0].first->printOperand(llvm::outs());

    llvm::outs() << ", " << this->operands[0].second << ">";

    llvm::outs() << "\n";
}

// Fence instruction
FenceInstruction::FenceInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to FENCE
    this->instruction_type = InstructionType::FENCE;

    if(llvm::isa<llvm::FenceInst>(this->instruction)) { }
    else   
        llvm_unreachable("[FenceInstruction Error] The underlying LLVM instruction is not a fence instruction!");
}

void FenceInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Atomic compare and change instruction
AtomicCompareChangeInstruction::AtomicCompareChangeInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to ATOMIC_COMPARE_CHANGE
    this->instruction_type = InstructionType::ATOMIC_COMPARE_CHANGE;

    llvm::AtomicCmpXchgInst *atomic_compare_change_inst;

    if (atomic_compare_change_inst = llvm::dyn_cast<llvm::AtomicCmpXchgInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) atomic_compare_change_inst;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);
        
        // 0 means that either the operand is a constant or the indirection level is not relevant
        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        llvm::Value *val_pointer_operand = atomic_compare_change_inst->getPointerOperand();

        SLIMOperand *pointer_slim_operand = OperandRepository::getSLIMOperand(val_pointer_operand);

        if (!pointer_slim_operand)
        {
            pointer_slim_operand = new SLIMOperand(val_pointer_operand);
            OperandRepository::setSLIMOperand(val_pointer_operand, pointer_slim_operand);
        }

        this->pointer_operand = std::make_pair(pointer_slim_operand, 1);

        llvm::Value *val_compare_operand = atomic_compare_change_inst->getCompareOperand();

        SLIMOperand *compare_slim_operand = OperandRepository::getSLIMOperand(val_compare_operand);

        if (!compare_slim_operand)
        {
            compare_slim_operand = new SLIMOperand(val_compare_operand);
            OperandRepository::setSLIMOperand(val_compare_operand, compare_slim_operand);
        }

        this->compare_operand = std::make_pair(compare_slim_operand, 0);

        llvm::Value *val_new_operand = atomic_compare_change_inst->getNewValOperand();

        SLIMOperand *val_new_slim_operand = OperandRepository::getSLIMOperand(val_new_operand);

        if (!val_new_slim_operand)
        {
            val_new_slim_operand = new SLIMOperand(val_new_operand);
            OperandRepository::setSLIMOperand(val_new_operand, val_new_slim_operand);
        }

        this->new_value = std::make_pair(val_new_slim_operand, 0);
    }
    else
    {
        llvm_unreachable("[AtomicCompareChangeInstruction Error] The underlying LLVM instruction is not an atomic compare and change instruction!");
    }
}

llvm::Value * AtomicCompareChangeInstruction::getPointerOperand()
{
    return this->pointer_operand.first->getValue();
}

llvm::Value * AtomicCompareChangeInstruction::getCompareOperand()
{
    return this->compare_operand.first->getValue();
}

llvm::Value * AtomicCompareChangeInstruction::getNewValue()
{
    return this->new_value.first->getValue();
}

void AtomicCompareChangeInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Atomic modify memory instruction
AtomicModifyMemInstruction::AtomicModifyMemInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to ATOMIC_MODIFY_MEM
    this->instruction_type = InstructionType::ATOMIC_MODIFY_MEM;

    if (llvm::isa<llvm::AtomicRMWInst>(this->instruction)) { }
    else
        llvm_unreachable("[AtomicModifyMemInstruction Error] The underlying LLVM instruction is not an AtomicRMW instruction!");
}

void AtomicModifyMemInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Getelementptr instruction
GetElementPtrInstruction::GetElementPtrInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to GET_ELEMENT_PTR
    this->instruction_type = InstructionType::GET_ELEMENT_PTR;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    llvm::GetElementPtrInst *get_element_ptr;

    if (get_element_ptr = llvm::dyn_cast<llvm::GetElementPtrInst>(this->instruction))
    {
        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        this->result = std::make_pair(result_slim_operand, 1);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);
            
            if (operand_i->stripPointerCasts())
            {
                operand_i = operand_i->stripPointerCasts();
            }

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }   

        SLIMOperand * gep_main_slim_operand;
        
        if (get_element_ptr->getPointerOperand()->stripPointerCasts())
        {
            gep_main_slim_operand = new SLIMOperand(get_element_ptr->getPointerOperand()->stripPointerCasts());
        } 
        else
        {
            gep_main_slim_operand = new SLIMOperand(get_element_ptr->getPointerOperand());
        }

        // Create and store the index operands into the indices list
        for (int i = 1; i < get_element_ptr->getNumOperands(); i++)
        {
            llvm::Value *index_val = get_element_ptr->getOperand(i);

            if (llvm::isa<llvm::Constant>(index_val))
            {
                if (llvm::isa<llvm::ConstantInt>(index_val))
                {
                    SLIMOperand *index_slim_operand = new SLIMOperand(index_val);
                    this->indices.push_back(index_slim_operand);
                }
                else
                {
                    llvm_unreachable("[GetElementPtrInstruction Error] The index is a constant but not an integer constant!");
                }
            }
            else
            {
                if (index_val != nullptr)
                {
                    llvm::Value * rhs_operand_after_strip = llvm::dyn_cast<llvm::Value>(index_val->stripPointerCasts());

                    if (rhs_operand_after_strip)
                    {
                        index_val = rhs_operand_after_strip;
                    }
                }

                SLIMOperand *index_slim_operand = new SLIMOperand(index_val);
                this->indices.push_back(index_slim_operand);
            }
        }
    }
    else
    {
        llvm_unreachable("[GetElementPtrInstruction Error] The underlying LLVM instruction is not a getelementptr instruction!");
    }
}

// Returns the main operand (corresponding to the aggregate name)
SLIMOperand * GetElementPtrInstruction::getMainOperand()
{
    return this->gep_main_operand;
}

// Returns the number of index operands
unsigned GetElementPtrInstruction::getNumIndexOperands()
{
    return this->indices.size();
}

// Returns the operand corresponding to the index at the given position (0-based)
SLIMOperand * GetElementPtrInstruction::getIndexOperand(unsigned position)
{
    // Make sure that the position is in bounds
    assert(position >= 0 && position < this->getNumIndexOperands());

    return this->indices[position];
}

void GetElementPtrInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    if (this->result.first->getValue())
    {
        llvm::outs() << "<";

        this->result.first->printOperand(llvm::outs());

        llvm::outs() << ", " << this->result.second << ">";
        
        llvm::outs() << " = ";
    }

    llvm::GetElementPtrInst *get_element_ptr;

    if (get_element_ptr = llvm::dyn_cast<llvm::GetElementPtrInst>(this->instruction))
    {
        llvm::outs() << "<";

        llvm::outs() << get_element_ptr->getPointerOperand()->getName();

        // if (gep_main_operand->getValue())
        // {
        //     gep_main_operand->printOperand(llvm::outs());
        //     llvm::outs() << "\n";
        // }

        for (int i = 1; i < get_element_ptr->getNumOperands(); i++)
        {
            llvm::Value *index_val = get_element_ptr->getOperand(i);
            
            if (llvm::isa<llvm::Constant>(index_val))
            {
                if (llvm::isa<llvm::ConstantInt>(index_val))
                {
                    llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(index_val);

                    llvm::outs() << "[" << constant_int->getSExtValue() << "]";
                }
                else
                {
                    llvm_unreachable("[GetElementPtrInstruction Error] The index is a constant but not an integer constant!");
                }
            }
            else
            {
                llvm::outs() << "[" << index_val->getName() << "]";
            }
        }

        // The indirection level (0) can be fetched from the individual indirection of indices as well
        llvm::outs() << ", 0>";
        llvm::outs() << "\n";
    }
    else
    {
        llvm_unreachable("[GetElementPtrInstruction Error] The underlying LLVM instruction is not a getelementptr instruction!");
    }
}

// Unary operations

// Floating-point negation instruction
FPNegationInstruction::FPNegationInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to FP_NEGATION
    this->instruction_type = InstructionType::FP_NEGATION;

    this->is_expression_assignment = true;

    llvm::Value *result_operand = (llvm::Value *) this->instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    llvm::Value *operand = this->instruction->getOperand(0);

    SLIMOperand *slim_operand = OperandRepository::getSLIMOperand(operand);

    if (!slim_operand)
    {
        slim_operand = new SLIMOperand(operand);
        OperandRepository::setSLIMOperand(operand, slim_operand);
    }    

    // 0 represents that either it is a constant or the indirection level is not relevant
    this->operands.push_back(std::make_pair(slim_operand, 0));
}

void FPNegationInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->result.first->printOperand(llvm::outs());

    llvm::outs() << " = ";

    this->operands[0].first->printOperand(llvm::outs());
    //Value *operand = this->operands[0].first->getValue();

    // if (isa<Constant>(operand))
    // {
    //     if (isa<ConstantFP>(operand))
    //     {
    //         ConstantFP *constant_fp = cast<ConstantFP>(operand);

    //         llvm::outs() << constant_fp->getValueAPF().convertToFloat();
    //     }
    //     else
    //     {
    //         llvm_unreachable("[FPNegationInstruction Error] The operand is a constant but not a floating-point constant!");
    //     }
    // }
    // else
    // {
    //     llvm::outs() << "-" << operand->getName();
    // }

    llvm::outs() << "\n";
}

// Binary operation
BinaryOperation::BinaryOperation(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to BINARY_OPERATION
    this->instruction_type = InstructionType::BINARY_OPERATION;

    this->is_expression_assignment = true;
    
    llvm::BinaryOperator *binary_operator;

    if (binary_operator = llvm::dyn_cast<llvm::BinaryOperator>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) binary_operator;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        // Set the operation type
        switch (binary_operator->getOpcode())
        {
            case llvm::Instruction::BinaryOps::Add: 
            case llvm::Instruction::BinaryOps::FAdd:
                                                this->binary_operator = ADD;
                                                break;
            case llvm::Instruction::BinaryOps::Sub:
            case llvm::Instruction::BinaryOps::FSub:
                                                this->binary_operator = SUB;
                                                break;
            case llvm::Instruction::BinaryOps::Mul:
            case llvm::Instruction::BinaryOps::FMul:
                                                this->binary_operator = MUL;
                                                break;
            case llvm::Instruction::BinaryOps::UDiv:
            case llvm::Instruction::BinaryOps::SDiv:
            case llvm::Instruction::BinaryOps::FDiv:
                                                this->binary_operator = DIV;
                                                break;
            case llvm::Instruction::BinaryOps::URem:
            case llvm::Instruction::BinaryOps::SRem:
            case llvm::Instruction::BinaryOps::FRem:
                                                this->binary_operator = REM;
                                                break;
            case llvm::Instruction::BinaryOps::Shl:
                                                this->binary_operator = SHIFT_LEFT;
                                                break;
            case llvm::Instruction::BinaryOps::LShr:
                                                this->binary_operator = LOGICAL_SHIFT_RIGHT;
                                                break;
            case llvm::Instruction::BinaryOps::AShr:
                                                this->binary_operator = ARITHMETIC_SHIFT_RIGHT;
                                                break;
            case llvm::Instruction::BinaryOps::And:
                                                this->binary_operator = BITWISE_AND;
                                                break;
            case llvm::Instruction::BinaryOps::Or:
                                                this->binary_operator = BITWISE_OR;
                                                break;
            case llvm::Instruction::BinaryOps::Xor:
                                                this->binary_operator = BITWISE_XOR;
                                                break;

            default: llvm_unreachable("[BinaryOperation Error] Unexpected binary operator!");
        }

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }   
    }
    else
    {
        llvm_unreachable("[BinaryOperation Error] The underlying LLVM instruction does not perform a binary operation!");
    }
}

SLIMBinaryOperator BinaryOperation::getOperationType()
{
    return this->binary_operator;
}

void BinaryOperation::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    for (int i = 0; i < this->operands.size(); i++)
    {
        this->operands[i].first->printOperand(llvm::outs());

        if (i != this->operands.size() - 1)
        {
            switch (this->getOperationType())
            {
                case ADD:   llvm::outs() << " + ";
                            break;
                case SUB:   llvm::outs() << " - ";
                            break;
                case MUL:   llvm::outs() << " * ";
                            break;
                case DIV:   llvm::outs() << " / ";
                            break;
                case REM:   llvm::outs() << " % ";
                            break;
                            
                case SHIFT_LEFT:                llvm::outs() << " << ";
                                                break;
                case LOGICAL_SHIFT_RIGHT:       llvm::outs() << " >>> ";
                                                break;
                case ARITHMETIC_SHIFT_RIGHT:    llvm::outs() << " >> ";
                                                break;
                case BITWISE_AND:               llvm::outs() << " & ";
                                                break;
                case BITWISE_OR:                llvm::outs() << " | ";
                                                break;
                case BITWISE_XOR:               llvm::outs() << " ^ ";
                                                break;

                default: llvm_unreachable("[BinaryOperation Error] Unexpected binary operation type!");
            }
        }
        else
        {
            llvm::outs() << "\n";
        }
    }
}

// Vector operations

// Extract element instruction
ExtractElementInstruction::ExtractElementInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to EXTRACT_ELEMENT
    this->instruction_type = InstructionType::EXTRACT_ELEMENT;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void ExtractElementInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->result.first->printOperand(llvm::outs());

    llvm::outs() << " = ";

    //Value *operand_0 = this->operands[0].first->getValue();
    this->operands[0].first->printOperand(llvm::outs());
    
    llvm::outs() << "[";

    //Value *operand_1 = this->operands[1].first->getValue();

    this->operands[1].first->printOperand(llvm::outs());

    llvm::outs() << "]\n";
}

// Insert element instruction
InsertElementInstruction::InsertElementInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to INSERT_ELEMENT
    this->instruction_type = InstructionType::INSERT_ELEMENT;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void InsertElementInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->result.first->printOperand(llvm::outs());

    llvm::outs() << " = ";

    this->operands[0].first->printOperand(llvm::outs());

    llvm::outs() << ".insert(";

    this->operands[1].first->printOperand(llvm::outs());

    llvm::outs() << ", ";

    this->operands[2].first->printOperand(llvm::outs());

    llvm::outs() << ")\n";
}

// ShuffleVector instruction
ShuffleVectorInstruction::ShuffleVectorInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to SHUFFLE_VECTOR
    this->instruction_type = InstructionType::SHUFFLE_VECTOR;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void ShuffleVectorInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::outs() << "shufflevector(";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << operand_0->getName() << ", ";

    llvm::Value *operand_1 = this->operands[1].first->getValue();

    llvm::outs() << operand_1->getName() << ", ";

    llvm::Value *operand_2 = this->operands[2].first->getValue();

    llvm::outs() << operand_2->getName() << ")\n";
}

// Operations for aggregates (structure and array) stored in registers

// ExtractValue instruction
ExtractValueInstruction::ExtractValueInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to EXTRACT_VALUE
    this->instruction_type = InstructionType::EXTRACT_VALUE;

    llvm::ExtractValueInst *extract_value_inst;

    if (extract_value_inst = llvm::dyn_cast<llvm::ExtractValueInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        // 0 represents that either it is a constant or the indirection level is not relevant    
        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);
        
        // Get aggregate operand
        llvm::Value *aggregate_operand = extract_value_inst->getAggregateOperand();
        
        // Get the SLIM operand object corresponding to the aggregate operand
        SLIMOperand *slim_aggregate_operand = OperandRepository::getSLIMOperand(aggregate_operand);

        // If the SLIM operand object does not exist, create it
        if (!slim_aggregate_operand)
        {
            slim_aggregate_operand = new SLIMOperand(aggregate_operand);
            OperandRepository::setSLIMOperand(aggregate_operand, slim_aggregate_operand);
        }

        this->operands.push_back(std::make_pair(slim_aggregate_operand, 0));
        
        // Insert the indices
        for (auto i : extract_value_inst->getIndices())
        {
            this->indices.push_back(i);
        }
    }
    else
    {
        llvm_unreachable("[ExtractValueInstruction Error] The underlying LLVM instruction is not a extractvalue instruction!");
    }
}

unsigned ExtractValueInstruction::getNumIndices()
{
    return this->indices.size();
}

unsigned ExtractValueInstruction::getIndex(unsigned index)
{
    assert(index >= 0 && index < this->getNumIndices());

    return this->indices[index];
}

void ExtractValueInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->result.first->printOperand(llvm::outs());

    llvm::outs() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << operand_0->getName();
    
    for (auto index : this->indices)
    {
        llvm::outs() << "[" << index << "]";
    }

    llvm::outs() << "\n";
}

// InsertValue instruction
InsertValueInstruction::InsertValueInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to INSERT_VALUE
    this->instruction_type = InstructionType::INSERT_VALUE;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void InsertValueInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0_aggregate_name = this->operands[0].first->getValue();

    llvm::outs() << operand_0_aggregate_name->getName();

    llvm::Value *operand_1_value_to_insert = this->operands[1].first->getValue();

    for (int i = 2; i < this->operands.size(); i++)
    {
        llvm::Value *operand_i = this->operands[i].first->getValue();

        if (llvm::isa<llvm::ConstantInt>(operand_i))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_i);

            llvm::outs() << "[" << constant_int->getSExtValue() << "]";
        }
        else
        {
            llvm_unreachable("[InsertValueInstruction Error] Unexpected index");
        }
    }

    llvm::outs() << ".insert(";

    this->operands[1].first->printOperand(llvm::outs());

    llvm::outs() << ")\n";
}

// Conversion operations

// Trunc instruction
TruncInstruction::TruncInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to TRUNC
    this->instruction_type = InstructionType::TRUNC;

    this->is_expression_assignment = true;

    if (llvm::isa<llvm::TruncInst>(this->instruction) || llvm::isa<llvm::FPTruncInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        // 0 represents that either it is a constant or the indirection level is not relevant    
        this->result = std::make_pair(result_slim_operand, 0);

        // Set the resulting type
        if (llvm::isa<llvm::TruncInst>(this->instruction))
            this->resulting_type = llvm::cast<llvm::TruncInst>(this->instruction)->getDestTy();
        else
            this->resulting_type = llvm::cast<llvm::FPTruncInst>(this->instruction)->getDestTy();

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }
    }
    else
    {
        llvm_unreachable("[TruncInstruction Error] The underlying LLVM instruction is not a trunc instruction!");
    }
}

llvm::Type * TruncInstruction::getResultingType()
{
    return this->resulting_type;
}

void TruncInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else if (llvm::isa<llvm::ConstantFP>(operand_0))
        {
            llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand_0);

            llvm::outs() << constant_float->getValueAPF().convertToFloat();
        }
        else
        {
            llvm::outs() << "[TruncInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();
    }

    llvm::outs() << "\n";
}

// Zext instruction
ZextInstruction::ZextInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to ZEXT
    this->instruction_type = InstructionType::ZEXT;

    this->is_expression_assignment = true;

    llvm::ZExtInst *zext_inst;

    if (zext_inst = llvm::dyn_cast<llvm::ZExtInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        // 0 represents that either it is a constant or the indirection level is not relevant    
        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        // Set the resulting type
        this->resulting_type = zext_inst->getDestTy();

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }
    }
    else
    {
        llvm_unreachable("[ZextInstruction Error] The underlying LLVM instruction is not a zext instruction!");
    }
}

llvm::Type * ZextInstruction::getResultingType()
{
    return this->resulting_type;
}

void ZextInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(Zext-";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else if (llvm::isa<llvm::ConstantFP>(operand_0))
        {
            llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand_0);

            llvm::outs() << constant_float->getValueAPF().convertToFloat();
        }
        else
        {
            llvm::outs() << "[ZextInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();

    }

    llvm::outs() << "\n";
}

// Sext instruction
SextInstruction::SextInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to SEXT
    this->instruction_type = InstructionType::SEXT;

    this->is_expression_assignment = true;

    llvm::SExtInst *sext_inst;
    
    if (sext_inst = llvm::dyn_cast<llvm::SExtInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        // 0 represents that either it is a constant or the indirection level is not relevant    
        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        // Set the resulting type
        this->resulting_type = sext_inst->getDestTy();

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }
    }
    else
    {
        llvm_unreachable("[SextInstruction Error] The underlying LLVM instruction is not a sext instruction!");
    }
}

llvm::Type * SextInstruction::getResultingType()
{
    return this->resulting_type;
}

void SextInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(SignExt-";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else
        {
            llvm::outs() << "[SextInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();

    }

    llvm::outs() << "\n";
}

// FPExt instruction
FPExtInstruction::FPExtInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to FPEXT
    this->instruction_type = InstructionType::FPEXT;

    this->is_expression_assignment = true;

    if (llvm::isa<llvm::FPExtInst>(this->instruction))
    {
        llvm::FPExtInst *fp_ext_inst = llvm::cast<llvm::FPExtInst>(this->instruction);

        this->resulting_type = fp_ext_inst->getDestTy();
    }
    else
    {
        llvm_unreachable("[IntToFPInstruction Error] The underlying LLVM instruction is not a fpext instruction!");
    }

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

llvm::Type * FPExtInstruction::getResultingType()
{
    return this->resulting_type;
}

void FPExtInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(FPExt-";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantFP>(operand_0))
        {
            llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand_0);

            llvm::outs() << constant_float->getValueAPF().convertToFloat();
        }
        else
        {
            llvm::outs() << "[FPExtInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();

    }

    llvm::outs() << "\n";
}

// FPToUi instruction
FPToIntInstruction::FPToIntInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to FP_TO_INT
    this->instruction_type = InstructionType::FP_TO_INT;

    this->is_expression_assignment = true;

    if (llvm::isa<llvm::FPToUIInst>(this->instruction))
    {
        this->resulting_type = llvm::cast<llvm::FPToUIInst>(this->instruction)->getDestTy();
    }
    else if (llvm::isa<llvm::FPToSIInst>(this->instruction))
    {
        this->resulting_type = llvm::cast<llvm::FPToSIInst>(this->instruction)->getDestTy();
    }
    else
    {
        llvm_unreachable("[FPToIntInstruction Error] The underlying LLVM instruction is not a FPToUI or FPToSI instruction!");
    }

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

llvm::Type * FPToIntInstruction::getResultingType()
{
    return this->resulting_type;
}

void FPToIntInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else if (llvm::isa<llvm::ConstantFP>(operand_0))
        {
            llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand_0);

            llvm::outs() << constant_float->getValueAPF().convertToFloat();
        }
        else
        {
            llvm::outs() << "[FPToIntInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();
    }

    llvm::outs() << "\n";
}

// IntToFP instruction
IntToFPInstruction::IntToFPInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to INT_TO_FP
    this->instruction_type = InstructionType::INT_TO_FP;

    this->is_expression_assignment = true;

    if (llvm::isa<llvm::UIToFPInst>(this->instruction))
    {
        llvm::UIToFPInst *ui_to_fp_inst = llvm::cast<llvm::UIToFPInst>(this->instruction);

        this->resulting_type = ui_to_fp_inst->getDestTy();
    }
    else if (llvm::isa<llvm::SIToFPInst>(this->instruction))
    {
        llvm::SIToFPInst *si_to_fp_inst = llvm::cast<llvm::SIToFPInst>(this->instruction);

        this->resulting_type = si_to_fp_inst->getDestTy();
    }
    else
    {
        llvm_unreachable("[IntToFPInstruction Error] The underlying LLVM instruction is not a integer-to-floating-point conversion instruction!");
    }

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

llvm::Type * IntToFPInstruction::getResultingType()
{
    return this->resulting_type;
}

void IntToFPInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else
        {
            llvm::outs() << "[IntToFPInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();
    }

    llvm::outs() << "\n";
}


// PtrToInt instruction
PtrToIntInstruction::PtrToIntInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to PTR_TO_INT
    this->instruction_type = InstructionType::PTR_TO_INT;

    this->is_expression_assignment = true;

    if (llvm::isa<llvm::PtrToIntInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        // 0 represents that either it is a constant or the indirection level is not relevant    
        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        this->resulting_type = llvm::cast<llvm::PtrToIntInst>(this->instruction)->getDestTy();

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }
    }
    else
    {
        llvm_unreachable("[PtrToIntInstruction Error] The underlying LLVM instruction is not a PtrToInt instruction!");
    }   
}

llvm::Type * PtrToIntInstruction::getResultingType()
{
    return this->resulting_type;
}

void PtrToIntInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else
        {
            llvm::outs() << "[PtrToIntInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();
    }

    llvm::outs() << "\n";
}

// IntToPtr instruction
IntToPtrInstruction::IntToPtrInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to INT_TO_PTR
    this->instruction_type = InstructionType::INT_TO_PTR;

    this->is_expression_assignment = true;

    if (llvm::isa<llvm::IntToPtrInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        // 0 represents that either it is a constant or the indirection level is not relevant    
        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        // Set the resulting type
        this->resulting_type = llvm::cast<llvm::IntToPtrInst>(this->instruction)->getDestTy();

        for (int i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value *operand_i = instruction->getOperand(i);

            SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

            if (!slim_operand_i)
            {
                slim_operand_i = new SLIMOperand(operand_i);
                OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
            }    

            // 0 represents that either it is a constant or the indirection level is not relevant
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }
    }
    else
    {
        llvm_unreachable("[IntToPtrInstruction Error] The underlying LLVM instruction is not a IntToPtr instruction!");
    }
}

llvm::Type * IntToPtrInstruction::getResultingType()
{
    return this->resulting_type;
}

void IntToPtrInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else
        {
            llvm::outs() << "[IntToPtrInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();
    }

    llvm::outs() << "\n";
}

// Bitcast instruction
BitcastInstruction::BitcastInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to BITCAST
    this->instruction_type = InstructionType::BITCAST;

    this->is_expression_assignment = true;

    llvm::BitCastInst *bitcast_inst;

    if (bitcast_inst = llvm::dyn_cast<llvm::BitCastInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) bitcast_inst;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        this->result = std::make_pair(result_slim_operand, 1);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        llvm::Value *operand_0 = instruction->getOperand(0);

        SLIMOperand *slim_operand_0 = OperandRepository::getSLIMOperand(operand_0);

        if (!slim_operand_0)
        {
            slim_operand_0 = new SLIMOperand(operand_0);
            OperandRepository::setSLIMOperand(operand_0, slim_operand_0);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_0, 1));

        this->resulting_type = bitcast_inst->getDestTy();
    }
    else
    {
        llvm_unreachable("[BitcastInstruction Error] The underlying LLVM instruction is not a bitcast instruction!");
    }
}

llvm::Type * BitcastInstruction::getResultingType()
{
    return this->resulting_type;
}

void BitcastInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(";

    this->getResultingType()->print(llvm::outs());

    llvm::outs() << ") ";

    this->operands[0].first->printOperand(llvm::outs());

    llvm::outs() << "\n";
}

// Address space instruction
AddrSpaceInstruction::AddrSpaceInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to ADDR_SPACE
    this->instruction_type = InstructionType::ADDR_SPACE;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void AddrSpaceInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *operand_0 = this->operands[0].first->getValue();

    llvm::outs() << "(address-space-cast-";

    llvm::Type *operand_1 = this->operands[1].first->getValue()->getType();

    operand_1->print(llvm::outs());

    llvm::outs() << ") ";

    if (llvm::isa<llvm::Constant>(operand_0))
    {
        if (llvm::isa<llvm::ConstantInt>(operand_0))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_0);

            llvm::outs() << constant_int->getSExtValue();
        }
        else
        {
            llvm::outs() << "[AddrSpaceInstruction Error] Unexpected constant!\n";
        }
    }
    else
    {
        llvm::outs() << operand_0->getName();
    }

    llvm::outs() << "\n";
}

// Other important instructions

// Compare instruction
CompareInstruction::CompareInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to COMPARE
    this->instruction_type = InstructionType::COMPARE;

    this->is_expression_assignment = true;
    
    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void CompareInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = ";

    llvm::Value *condition_operand = this->operands[0].first->getValue();

    llvm::Value *operand_1 = this->operands[0].first->getValue();

    llvm::Value *operand_2 = this->operands[1].first->getValue();

    this->operands[0].first->printOperand(llvm::outs());
    // if (isa<Constant>(operand_1))
    // {
    //     if (cast<Constant>(operand_1)->isNullValue())
    //     {
    //         llvm::outs() << "null";
    //     }
    //     else if (isa<ConstantInt>(operand_1))
    //     {
    //         ConstantInt *constant_int = cast<ConstantInt>(operand_1);

    //         llvm::outs() << constant_int->getSExtValue();
    //     }
    //     else if (isa<ConstantFP>(operand_1))
    //     {
    //         ConstantFP *constant_float = cast<ConstantFP>(operand_1);

    //         llvm::outs() << constant_float->getValueAPF().convertToFloat();
    //     }
    //     else
    //     {
    //         llvm_unreachable("[CompareInstruction Error] Unexpected constant!");
    //     }
    // }
    // else
    // {
    //     llvm::outs() << operand_1->getName();
    // }

    if (llvm::isa<llvm::ICmpInst>(this->instruction))
    {
        llvm::ICmpInst *icmp_instruction = llvm::cast<llvm::ICmpInst>(this->instruction);

        llvm::CmpInst::Predicate predicate = icmp_instruction->getPredicate();
        
        switch (predicate)
        {
            case llvm::CmpInst::ICMP_EQ: llvm::outs() << " == ";
                                         break;
            
            case llvm::CmpInst::ICMP_NE: llvm::outs() << " != ";
                                         break;
            
            case llvm::CmpInst::ICMP_UGT:
            case llvm::CmpInst::ICMP_SGT: llvm::outs() << " > ";
                                          break;
            
            case llvm::CmpInst::ICMP_UGE:
            case llvm::CmpInst::ICMP_SGE: llvm::outs() << " >= ";
                                          break;

            case llvm::CmpInst::ICMP_ULT:
            case llvm::CmpInst::ICMP_SLT: llvm::outs() << " < ";
                                          break;
            
            case llvm::CmpInst::ICMP_ULE:
            case llvm::CmpInst::ICMP_SLE: llvm::outs() << " <= ";
                                          break;

            default: llvm_unreachable("[CompareInstruction Error] Unexpected predicate!");
        }
    }
    else if (llvm::isa<llvm::FCmpInst>(this->instruction))
    {
        llvm::FCmpInst *fcmp_instruction = llvm::cast<llvm::FCmpInst>(this->instruction);

        llvm::FCmpInst::Predicate predicate = fcmp_instruction->getPredicate();

        switch (predicate)
        {
            case llvm::CmpInst::FCMP_OEQ:
            case llvm::CmpInst::FCMP_UEQ: llvm::outs() << " == ";
                                          break;
            
            case llvm::CmpInst::FCMP_ONE:
            case llvm::CmpInst::FCMP_UNE: llvm::outs() << " != ";
                                          break;
            
            case llvm::CmpInst::FCMP_OGT:
            case llvm::CmpInst::FCMP_UGT: llvm::outs() << " > ";
                                          break;
            
            case llvm::CmpInst::FCMP_OGE:
            case llvm::CmpInst::FCMP_UGE: llvm::outs() << " >= ";
                                          break;

            case llvm::CmpInst::FCMP_OLT:
            case llvm::CmpInst::FCMP_ULT: llvm::outs() << " < ";
                                          break;
            
            case llvm::CmpInst::FCMP_OLE:
            case llvm::CmpInst::FCMP_ULE: llvm::outs() << " <= ";
                                          break;

            // True if both the operands are not QNAN
            case llvm::CmpInst::FCMP_ORD: llvm::outs() << " !QNAN";
                                          break;

            // True if either of the operands is/are a QNAN
            case llvm::CmpInst::FCMP_UNO: llvm::outs() << " EITHER-QNAN ";
                                          break;

            case llvm::CmpInst::FCMP_FALSE: llvm::outs() << " false ";
                                            break;
            
            case llvm::CmpInst::FCMP_TRUE: llvm::outs() << " true ";
                                           break;

            default: llvm_unreachable("[CompareInstruction Error] Unexpected predicate!");
        }
    }
    else
    {
        llvm_unreachable("[CompareInstruction Error] Unexpected compare instruction type!");
    }

    // if (isa<Constant>(operand_2))
    // {
    //     if (cast<Constant>(operand_2)->isNullValue())
    //     {
    //         llvm::outs() << "null";
    //     }
    //     else if (isa<ConstantInt>(operand_2))
    //     {
    //         ConstantInt *constant_int = cast<ConstantInt>(operand_2);

    //         llvm::outs() << constant_int->getSExtValue();
    //     }
    //     else if (isa<ConstantFP>(operand_2))
    //     {
    //         ConstantFP *constant_float = cast<ConstantFP>(operand_2);

    //         llvm::outs() << constant_float->getValueAPF().convertToFloat();
    //     }
    //     else
    //     {
    //         llvm_unreachable("[CompareInstruction Error] Unexpected constant!");
    //     }
    // }
    // else
    // {
    //     llvm::outs() << operand_2->getName();
    // }

    this->operands[1].first->printOperand(llvm::outs());

    llvm::outs() << "\n";
}

// Phi instruction
PhiInstruction::PhiInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to PHI
    this->instruction_type = InstructionType::PHI;

    this->is_expression_assignment = true;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    llvm::PHINode *phi_inst = llvm::cast<llvm::PHINode>(instruction);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 1);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < phi_inst->getNumIncomingValues(); i++)
    {
        llvm::Value *operand_i = phi_inst->getIncomingValue(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        if (slim_operand_i->isPointerVariable())
        {
            this->operands.push_back(std::make_pair(slim_operand_i, 1));
        }
        else
        {
            this->operands.push_back(std::make_pair(slim_operand_i, 0));
        }
    }
}

void PhiInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = phi(";

    for (int i = 0; i < this->operands.size(); i++)
    {
        this->operands[i].first->printOperand(llvm::outs());
     
        if (i != this->operands.size() - 1)
        {
            llvm::outs() << ", ";
        }
        else
        {
            llvm::outs() << ")\n";
        }
    }
}

// Select instruction
SelectInstruction::SelectInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to SELECT
    this->instruction_type = InstructionType::SELECT;

    this->is_expression_assignment = true;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void SelectInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::SelectInst *select_instruction;

    if (select_instruction = llvm::dyn_cast<llvm::SelectInst>(this->instruction))
    {
        // Has three operands : condition, true-operand (operand that will be assigned when the condition is true)
        // and false-operand (operand that will be assigned when the condition is false)
        SLIMOperand *condition = this->operands[0].first;

        SLIMOperand *true_operand = this->operands[1].first;

        SLIMOperand *false_operand = this->operands[2].first;

        llvm::outs() << this->result.first->getValue()->getName() << " = ";

        // Print the condition operand
        condition->printOperand(llvm::outs());

        llvm::outs() << " ? ";

        // Print the "true" operand
        true_operand->printOperand(llvm::outs());

        llvm::outs() << " : ";

        // Print the "false" operand
        false_operand->printOperand(llvm::outs());
        
        llvm::outs() << "\n";
    }
    else
    {
        llvm_unreachable("[SelectInstruction Error] The corresponding LLVM instruction is not a select instruction!");
    }
}

// Freeze instruction
FreezeInstruction::FreezeInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to FREEZE
    this->instruction_type = InstructionType::FREEZE;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    // Has only single operand
    llvm::Value *operand = instruction->getOperand(0);

    SLIMOperand *slim_operand = OperandRepository::getSLIMOperand(operand);

    if (!slim_operand)
    {
        slim_operand = new SLIMOperand(operand);
        OperandRepository::setSLIMOperand(operand, slim_operand);
    }

    // 0 represents that either it is a constant or the indirection level is not relevant
    this->operands.push_back(std::make_pair(slim_operand, 0));
}

void FreezeInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << this->result.first->getValue()->getName() << " = freeze(";

    llvm::Value *operand = this->operands[0].first->getValue();

    llvm::outs() << operand->getName();
    
    llvm::outs() << ")\n";
}

// Call instruction
CallInstruction::CallInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to CALL
    this->instruction_type = InstructionType::CALL;

    llvm::CallInst *call_instruction;

    if (call_instruction = llvm::dyn_cast<llvm::CallInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) call_instruction;

        // Store the callee function (returns NULL if it is an indirect call)
        this->callee_function = call_instruction->getCalledFunction();
        
        if (!this->callee_function)
        {
            this->callee_function = llvm::dyn_cast<llvm::Function>(call_instruction->getCalledOperand()->stripPointerCasts());    
        }

        this->indirect_call = false;
        this->indirect_call_operand = NULL;
        
        if (!this->callee_function)
        {
            if (call_instruction->isIndirectCall())
            {
                this->indirect_call = true;
                llvm::Value *called_operand = call_instruction->getCalledOperand();
                this->indirect_call_operand = OperandRepository::getSLIMOperand(called_operand);

                if (!this->indirect_call_operand)
                {
                    this->indirect_call_operand = new SLIMOperand(called_operand);
                    OperandRepository::setSLIMOperand(called_operand, indirect_call_operand);
                }
            }
            else
            {
                llvm_unreachable("[CallInstruction Error] This call instruction is neither a direct call not an indirect call (unexpected error)!");
            }
        }

        

        if (!this->indirect_call)
        {
            // Check if this call instruction corresponds to either scanf, sscanf, or fscanf

            if (this->callee_function->getName() == "__isoc99_sscanf")
            {
                this->input_statement_type = InputStatementType::SSCANF;
                //this->is_input_statement = true;
                this->starting_input_args_index = 2;
            }
            else if (this->callee_function->getName() == "__isoc99_fscanf")
            {
                this->input_statement_type = InputStatementType::FSCANF;
                //this->is_input_statement = true;
                this->starting_input_args_index = 2;
            }
            else if (this->callee_function->getName() == "__isoc99_scanf")
            {
                this->input_statement_type = InputStatementType::SCANF;
                //this->is_input_statement = true;
                this->starting_input_args_index = 1;
            }

            SLIMOperand *result_slim_operand = new SLIMOperand(result_operand, false, this->callee_function);
            this->result = std::make_pair(result_slim_operand, 0);
            OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

            for (auto arg = this->callee_function->arg_begin(); arg != this->callee_function->arg_end(); arg++)
            {
                this->formal_arguments_list.push_back(arg);
            }
        }
        else
        {
            SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);
            this->result = std::make_pair(result_slim_operand, 0);
            OperandRepository::setSLIMOperand(result_operand, result_slim_operand);
        }

        for (unsigned i = 0; i < call_instruction->arg_size(); i++)
        {
            // Get the ith argument of the call instruction
            llvm::Value *arg_i = call_instruction->getArgOperand(i);

            // Check whether the corresponding SLIM operand already exists or not (and store, if it exists)
            SLIMOperand *arg_i_slim_operand = OperandRepository::getSLIMOperand(arg_i);

            // If the SLIM operand does not exist, create a new one and store the corresponding mapping in the
            // operand repository
            if (!arg_i_slim_operand)
            {
                arg_i_slim_operand = new SLIMOperand(arg_i);
                OperandRepository::setSLIMOperand(arg_i, arg_i_slim_operand);
            }

            // Push the argument operand in the operands vector
            // The indirection level is not relevant so it is 0 for now
            this->operands.push_back(std::make_pair(arg_i_slim_operand, 0));
        }
    }
    else
    {
        llvm_unreachable("[CallInstruction Error] The underlying LLVM instruction is not a call instruction!");
    }
}

// Returns whether the call is an indirect call or a direct call
bool CallInstruction::isIndirectCall()
{
    return this->indirect_call;
}

// Returns the indirect call SLIM operand
SLIMOperand * CallInstruction::getIndirectCallOperand()
{
    return this->indirect_call_operand;
}

// Return the callee function
llvm::Function * CallInstruction::getCalleeFunction()
{
    return this->callee_function;
}

// Returns the number of formal arguments in this call
unsigned CallInstruction::getNumFormalArguments()
{
    return this->formal_arguments_list.size();
}

// Returns the formal argument at a particular index
llvm::Argument * CallInstruction::getFormalArgument(unsigned index)
{
    // The index should be not be out-of-bounds
    assert(index >= 0 && index < this->getNumFormalArguments());

    return this->formal_arguments_list[index];
}

void CallInstruction::printInstruction()
{
    const std::string dbg_declare = "llvm.dbg.declare";
    const std::string dbg_value = "llvm.dbg.value";
    
    // If callee_function is NULL then it is an indirect calll
    if (!this->isIndirectCall())
    {
        llvm::StringRef callee_function_name = this->callee_function->getName();

        if (callee_function_name.str() == (dbg_declare) || callee_function_name.str() == (dbg_value))
        {
            return ;
        }
    }

    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    if (!this->result.first->getValue()->getName().empty())
        llvm::outs() << this->result.first->getValue()->getName() << " = call ";


    // If callee_function is NULL then it is an indirect calll
    if (!this->isIndirectCall())
    {
        llvm::outs() << this->callee_function->getName() << "(";
    }
    else
    {
        llvm::outs() << "(";

        this->getIndirectCallOperand()->printOperand(llvm::outs());

        llvm::outs() << ") (";
    }

    if (this->operands.empty())
    {
        llvm::outs() << ")\n";
    }

    for (int i = 0; i < this->operands.size(); i++)
    {
        this->operands[i].first->printOperand(llvm::outs());

        if (i != this->operands.size() - 1)
        {
            llvm::outs() << ", ";
        }
        else
        {
            llvm::outs() << ")\n";
        }
    }
}

// Variable argument instruction
VarArgInstruction::VarArgInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to VAR_ARG
    this->instruction_type = InstructionType::VAR_ARG;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void VarArgInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Landingpad instruction
LandingpadInstruction::LandingpadInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to LANDING_PAD
    this->instruction_type = InstructionType::LANDING_PAD;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void LandingpadInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Catchpad instruction
CatchpadInstruction::CatchpadInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to CATCH_PAD
    this->instruction_type = InstructionType::CATCH_PAD;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);

        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void CatchpadInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Cleanuppad instruction
CleanuppadInstruction::CleanuppadInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to CLEANUP_PAD
    this->instruction_type = InstructionType::CLEANUP_PAD;

    llvm::Value *result_operand = (llvm::Value *) instruction;

    SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

    // 0 represents that either it is a constant or the indirection level is not relevant    
    this->result = std::make_pair(result_slim_operand, 0);

    OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

    for (int i = 0; i < instruction->getNumOperands(); i++)
    {
        llvm::Value *operand_i = instruction->getOperand(i);
        
        SLIMOperand *slim_operand_i = OperandRepository::getSLIMOperand(operand_i);

        if (!slim_operand_i)
        {
            slim_operand_i = new SLIMOperand(operand_i);
            OperandRepository::setSLIMOperand(operand_i, slim_operand_i);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand_i, 0));
    }
}

void CleanuppadInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Return instruction
ReturnInstruction::ReturnInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to RETURN
    this->instruction_type = InstructionType::RETURN;

    llvm::ReturnInst *return_inst;

    if (return_inst = llvm::dyn_cast<llvm::ReturnInst>(this->instruction))
    {
        llvm::Value *temp_return_value = return_inst->getReturnValue();

        SLIMOperand *slim_return_value = OperandRepository::getSLIMOperand(temp_return_value);

        if (!slim_return_value)
        {
            slim_return_value = new SLIMOperand(temp_return_value);
            OperandRepository::setSLIMOperand(temp_return_value, slim_return_value);
        }

        this->return_value = slim_return_value;

        this->operands.push_back(std::make_pair(slim_return_value, 0));
    }
    else
    {
        llvm_unreachable("[ReturnInstruction Error] The underlying LLVM instruction is not a return instruction!");
    }
}

// Returns the SLIMOperand
SLIMOperand * ReturnInstruction::getReturnOperand()
{
    return this->return_value;
}

// Returns the Value * return operand
llvm::Value * ReturnInstruction::getReturnValue()
{
    return this->return_value->getValue();
}

void ReturnInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << "return ";

    llvm::Value *value = this->getReturnValue();

    if (!value)
    {
        llvm::outs() << "\n";
        return ;
    }

    SLIMOperand *value_slim_operand = new SLIMOperand(value);

    // Print the return operand
    value_slim_operand->printOperand(llvm::outs());

    llvm::outs() << "\n";
}

// Branch instruction
BranchInstruction::BranchInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to BRANCH
    this->instruction_type = InstructionType::BRANCH;

    llvm::BranchInst *branch_instruction;

    if (branch_instruction = llvm::dyn_cast<llvm::BranchInst>(this->instruction))
    {
        if (branch_instruction->isUnconditional())
        {
            this->is_conditional = false;
        }
        else
        {
            this->is_conditional = true;

            llvm::Value *condition_operand = branch_instruction->getCondition();

            SLIMOperand *condition_slim_operand = new SLIMOperand(condition_operand);

            this->operands.push_back(std::make_pair(condition_slim_operand, 0));
        }
    }
    else
    {
        llvm_unreachable("[BranchInstruction Error] The underlying LLVM instruction is not a branch instruction!");
    }
}

void BranchInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::BranchInst *branch_inst;

    llvm::outs() << "branch ";

    if (branch_inst = llvm::dyn_cast<llvm::BranchInst>(this->instruction))
    {
        if (this->is_conditional)
        {
            // The branch instruction is conditional, meaning there is only one successor

            llvm::Value *condition = branch_inst->getCondition();

            llvm::outs() << "(" << condition->getName() << ") ";
            llvm::outs() << branch_inst->getSuccessor(0)->getName();
            llvm::outs() << ", ";
            llvm::outs() << branch_inst->getSuccessor(1)->getName();
        }
        else
        {
            llvm::outs() << branch_inst->getSuccessor(0)->getName();
        }

        llvm::outs() << "\n";
    }
    else
    {
        llvm_unreachable("[BranchInstruction Error] The underlying LLVM instruction is not a branch instruction!");
    }
}

// Switch instruction
SwitchInstruction::SwitchInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to SWITCH
    this->instruction_type = InstructionType::SWITCH;

    llvm::SwitchInst *switch_instruction;

    if (switch_instruction = llvm::dyn_cast<llvm::SwitchInst>(this->instruction))
    {
        // First operand is the comparison value
        llvm::Value *comparison_value = switch_instruction->getCondition();

        SLIMOperand *comparison_slim_operand = OperandRepository::getSLIMOperand(comparison_value);

        if (!comparison_slim_operand)
        {
            comparison_slim_operand = new SLIMOperand(comparison_value);
            OperandRepository::setSLIMOperand(comparison_value, comparison_slim_operand);
        }    

        // Set the condition value
        this->condition_value = comparison_slim_operand;

        this->operands.push_back(std::make_pair(condition_value, 0));

        // Set the default destination
        this->default_case = switch_instruction->getDefaultDest();

        for (unsigned int i = 0; i <= switch_instruction->getNumCases(); i++)
        {
            llvm::BasicBlock *case_destination = switch_instruction->getSuccessor(i);

            llvm::ConstantInt *case_value = switch_instruction->findCaseDest(case_destination);

            if (case_value)
            {
                this->other_cases.push_back(std::make_pair(case_value, case_destination));
            }
        }
    }
    else
    {
        llvm_unreachable("[SwitchInstruction Error] The underlying LLVM instruction is not a switch instruction!");
    }
}

SLIMOperand * SwitchInstruction::getConditionOperand()
{
    return this->condition_value;
}

llvm::BasicBlock * SwitchInstruction::getDefaultDestination()
{
    return this->default_case;
}

unsigned SwitchInstruction::getNumberOfCases()
{
    return this->other_cases.size();
}

llvm::ConstantInt * SwitchInstruction::getConstantOfCase(unsigned case_number)
{
    return this->other_cases[case_number].first;
}

llvm::BasicBlock * SwitchInstruction::getDestinationOfCase(unsigned case_number)
{
    return this->other_cases[case_number].second;
}

void SwitchInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::SwitchInst *switch_instruction;

    if (switch_instruction = llvm::dyn_cast<llvm::SwitchInst>(this->instruction))
    {
        llvm::outs() << "switch(";

        llvm::Value *comparison_value = this->getConditionOperand()->getValue();

        if (llvm::isa<llvm::Constant>(comparison_value))
        {
            if (llvm::isa<llvm::ConstantInt>(comparison_value))
            {
                llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(comparison_value);

                llvm::outs() << constant_int->getSExtValue();
            }
            else
            {
                llvm_unreachable("[SwitchInstruction Error] The comparison value is not an integer!");
            }
        }
        else
        {
            llvm::outs() << comparison_value->getName();
        }

        llvm::outs() << ") {\n";

        // Print non-default cases
        for (unsigned i = 0; i < this->getNumberOfCases(); i++)
        {
            llvm::ConstantInt *case_value = this->getConstantOfCase(i);

            llvm::outs() << "case " << case_value->getSExtValue() << ": branch-to ";

            llvm::BasicBlock *case_destination = this->getDestinationOfCase(i);

            llvm::outs() << case_destination->getName() << "; break; \n"; 
        }

        // Print default case
        llvm::outs() << "default: branch-to " << this->getDefaultDestination()->getName() << "\n"; 
    }
    else
    {
        llvm_unreachable("[SwitchInstruction Error] The underlying LLVM instruction is not a switch instruction!");
    }

    llvm::outs() << "}\n";
}

// Indirect branch instruction
IndirectBranchInstruction::IndirectBranchInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to INDIRECT_BRANCH
    this->instruction_type = InstructionType::INDIRECT_BRANCH;

    llvm::IndirectBrInst *indirect_br_inst;

    if (indirect_br_inst = llvm::dyn_cast<llvm::IndirectBrInst>(this->instruction))
    {
        this->address = indirect_br_inst->getAddress();

        for (unsigned i = 0; i < indirect_br_inst->getNumDestinations(); i++)
        {
            llvm::BasicBlock *possible_destination = indirect_br_inst->getDestination(i);
            this->possible_destinations.push_back(possible_destination);
        }
    }
    else
    {
        llvm_unreachable("[IndirectBranchInstruction Error] The underlying LLVM instruction is not a indirect branch instruction!");
    }
}

llvm::Value * IndirectBranchInstruction::getBranchAddress()
{
    return this->address;
}

unsigned IndirectBranchInstruction::getNumPossibleDestinations()
{
    return this->possible_destinations.size();
}

llvm::BasicBlock *IndirectBranchInstruction::getPossibleDestination(unsigned index)
{
    // Check whether the index lies in the correct bounds or not
    assert(index >= 0 && index < this->possible_destinations.size());
    
    return this->possible_destinations[index];
}

void IndirectBranchInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << "indirect branch to " << this->getBranchAddress()->getName();

    llvm::outs() << " : [";

    unsigned total_possible_dests = this->getNumPossibleDestinations();

    for (unsigned i = 0; i < total_possible_dests; i++)
    {
        llvm::outs() << this->getPossibleDestination(i)->getName();

        if (i != total_possible_dests - 1)
        {
            llvm::outs() << ", ";
        }
    }

    llvm::outs() << "]\n";
}

// Invoke instruction
InvokeInstruction::InvokeInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to INVOKE
    this->instruction_type = InstructionType::INVOKE;

    llvm::InvokeInst *invoke_inst;

    if (invoke_inst = llvm::dyn_cast<llvm::InvokeInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) invoke_inst;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        // Store the callee function (returns NULL if it is an indirect call)
        this->callee_function = invoke_inst->getCalledFunction();

        if (!this->callee_function)
        {
            this->callee_function = llvm::dyn_cast<llvm::Function>(invoke_inst->getCalledOperand()->stripPointerCasts());    
        }

        this->indirect_call = false;
        this->indirect_call_operand = NULL;

        if (!this->callee_function)
        {
            this->indirect_call = true;
            llvm::Value *called_operand = invoke_inst->getCalledOperand();
            this->indirect_call_operand = OperandRepository::getSLIMOperand(called_operand);

            if (!this->indirect_call_operand)
            {
                this->indirect_call_operand = new SLIMOperand(called_operand);
                OperandRepository::setSLIMOperand(called_operand, indirect_call_operand);
            }
        }

        // Store the normal destination
        this->normal_destination = invoke_inst->getNormalDest();

        // Store the exception destination (destination in case of exception handling)
        this->exception_destination = invoke_inst->getUnwindDest();

        for (unsigned i = 0; i < invoke_inst->arg_size(); i++)
        {
            // Get the ith argument of the invoke instruction
            llvm::Value *arg_i = invoke_inst->getArgOperand(i);

            // Check whether the corresponding SLIM operand already exists or not (and store, if it exists)
            SLIMOperand *arg_i_slim_operand = OperandRepository::getSLIMOperand(arg_i);

            // If the SLIM operand does not exist, create a new one and store the corresponding mapping in the
            // operand repository
            if (!arg_i_slim_operand)
            {
                arg_i_slim_operand = new SLIMOperand(arg_i);
                OperandRepository::setSLIMOperand(arg_i, arg_i_slim_operand);
            }

            // Push the argument operand in the operands vector
            // The indirection level is not relevant so it is 0 for now
            this->operands.push_back(std::make_pair(arg_i_slim_operand, 0));
        }
    }
    else
    {
        llvm_unreachable("[InvokeInstruction Error] The underlying LLVM instruction is not an invoke instruction!");
    }
}

// Returns whether the call is an indirect call or not
bool InvokeInstruction::isIndirectCall()
{
    return this->indirect_call;
}

// Returns a non-NULL SLIMOperand if the call is an indirect call
SLIMOperand * InvokeInstruction::getIndirectCallOperand()
{
    return this->indirect_call_operand;
}

// Returns a non-NULL llvm::Function* if the call is a direct call
llvm::Function *InvokeInstruction::getCalleeFunction()
{
    return this->callee_function;
}

llvm::BasicBlock *InvokeInstruction::getNormalDestination()
{
    return this->normal_destination;
}

llvm::BasicBlock *InvokeInstruction::getExceptionDestination()
{
    return this->exception_destination;
}

void InvokeInstruction::printInstruction()
{
    const std::string dbg_declare = "llvm.dbg.declare";
    const std::string dbg_value = "llvm.dbg.value";
    
    // If callee_function is NULL then it is an indirect calll
    if (!this->isIndirectCall())
    {
        llvm::StringRef callee_function_name = this->callee_function->getName();

        if (callee_function_name.str() == (dbg_declare) || callee_function_name.str() == (dbg_value))
        {
            return ;
        }
    }

    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    if (!this->result.first->getValue()->getName().empty())
        llvm::outs() << this->result.first->getValue()->getName() << " = invoke ";


    // If callee_function is NULL then it is an indirect calll
    if (!this->isIndirectCall())
    {
        llvm::outs() << this->callee_function->getName() << "(";
    }
    else
    {
        llvm::outs() << "(";

        this->getIndirectCallOperand()->printOperand(llvm::outs());

        llvm::outs() << ") (";
    }

    if (this->operands.empty())
    {
        llvm::outs() << ")\n";
    }

    for (int i = 0; i < this->operands.size(); i++)
    {
        this->operands[i].first->printOperand(llvm::outs());

        if (i != this->operands.size() - 1)
        {
            llvm::outs() << ", ";
        }
        else
        {
            llvm::outs() << ") ";
        }
    }

    llvm::outs() << this->getNormalDestination()->getName() << ", " << this->getExceptionDestination()->getName();

    llvm::outs() << "\n";
}

// Callbr instruction
CallbrInstruction::CallbrInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to CALL_BR
    this->instruction_type = InstructionType::CALL_BR;

    llvm::CallBrInst *callbr_instruction;

    if (callbr_instruction = llvm::dyn_cast<llvm::CallBrInst>(this->instruction))
    {
        llvm::Value *result_operand = (llvm::Value *) callbr_instruction;

        SLIMOperand *result_slim_operand = new SLIMOperand(result_operand);

        this->result = std::make_pair(result_slim_operand, 0);

        OperandRepository::setSLIMOperand(result_operand, result_slim_operand);

        // Store the callee function
        this->callee_function = callbr_instruction->getCalledFunction();

        // Store the default destination
        this->default_destination = callbr_instruction->getDefaultDest();

        // Store the indirect destinations
        for (unsigned i = 0; i < callbr_instruction->getNumIndirectDests(); i++)
        {
            this->indirect_destinations.push_back(callbr_instruction->getIndirectDest(i));
        }

        for (unsigned i = 0; i < callbr_instruction->arg_size(); i++)
        {
            // Get the ith argument of the call instruction
            llvm::Value *arg_i = callbr_instruction->getArgOperand(i);

            // Check whether the corresponding SLIM operand already exists or not (and store, if it exists)
            SLIMOperand *arg_i_slim_operand = OperandRepository::getSLIMOperand(arg_i);

            // If the SLIM operand does not exist, create a new one and store the corresponding mapping in the
            // operand repository
            if (!arg_i_slim_operand)
            {
                arg_i_slim_operand = new SLIMOperand(arg_i);
                OperandRepository::setSLIMOperand(arg_i, arg_i_slim_operand);
            }

            // Push the argument operand in the operands vector
            // The indirection level is not relevant so it is 0 for now
            this->operands.push_back(std::make_pair(arg_i_slim_operand, 0));
        }
    }
    else
    {
        llvm_unreachable("[CallbrInstruction Error] The underlying LLVM instruction is not a call instruction!");
    }
}

llvm::Function * CallbrInstruction::getCalleeFunction()
{
    return this->callee_function;
}

llvm::BasicBlock * CallbrInstruction::getDefaultDestination()
{
    return this->default_destination;
}

unsigned CallbrInstruction::getNumIndirectDestinations()
{
    return this->indirect_destinations.size();
}

llvm::BasicBlock * CallbrInstruction::getIndirectDestination(unsigned index)
{
    assert(index >= 0 && index < this->getNumIndirectDestinations());

    return this->indirect_destinations[index];
}

void CallbrInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    if (!this->result.first->getValue()->getName().empty())
        llvm::outs() << this->result.first->getValue()->getName() << " = callbr ";

    const std::string dbg_declare = "llvm.dbg.declare";
    const std::string dbg_value = "llvm.dbg.value";

    llvm::StringRef callee_function_name = this->callee_function->getName();

    if (callee_function_name.str() == (dbg_declare) || callee_function_name.str() == (dbg_value))
        return ;

    llvm::outs() << this->callee_function->getName() << "(";

    for (int i = 0; i < this->operands.size(); i++)
    {
        llvm::Value *operand_i = this->operands[i].first->getValue();

        if (llvm::isa<llvm::Constant>(operand_i))
        {
            llvm::GEPOperator *gep_operator;

            if (llvm::isa<llvm::ConstantInt>(operand_i))
            {
                llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand_i);

                llvm::outs() << constant_int->getSExtValue();
            }
            else if (llvm::isa<llvm::ConstantFP>(operand_i))
            {
                llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand_i);

                llvm::outs() << constant_float->getValueAPF().convertToFloat();
            }        
            else if (gep_operator = llvm::dyn_cast<llvm::GEPOperator>(operand_i))
            {
                llvm::Value *operand_i = gep_operator->getOperand(0);

                llvm::outs() << operand_i->getName();
            }
            else if (llvm::isa<llvm::GlobalValue>(operand_i))
            {
                llvm::outs() << llvm::cast<llvm::GlobalValue>(operand_i)->getName();
            }
            else
            {
                //llvm_unreachable("[CallInstruction Error] Unexpected constant!\n");
                llvm::outs() << "[InvokeInstruction Error] Unexpected constant!\n";
            }
        }
        else
        {
            llvm::outs() << operand_i->getName();
        }

        if (i != this->operands.size() - 1)
        {
            llvm::outs() << ", ";
        }
        else
        {
            llvm::outs() << ")";
        }
    }

    llvm::outs() << this->getDefaultDestination()->getName() << ", [";

    for (unsigned i = 0; i < this->getNumIndirectDestinations(); i++)
    {
        llvm::outs() << this->getIndirectDestination(i)->getName();

        if (i != this->getNumIndirectDestinations() - 1)
        {
            llvm::outs() << ", ";
        }
    }

    llvm::outs() << "]\n";
}

// Resume instruction - resumes propagation of an existing exception
ResumeInstruction::ResumeInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to RESUME
    this->instruction_type = InstructionType::RESUME;

    llvm::ResumeInst *resume_inst;

    if (resume_inst = llvm::dyn_cast<llvm::ResumeInst>(this->instruction))
    {
        llvm::Value *operand = resume_inst->getValue();

        SLIMOperand *slim_operand = OperandRepository::getSLIMOperand(operand);

        if (!slim_operand)
        {
            slim_operand = new SLIMOperand(operand);
            OperandRepository::setSLIMOperand(operand, slim_operand);
        }    

        // 0 represents that either it is a constant or the indirection level is not relevant
        this->operands.push_back(std::make_pair(slim_operand, 0));
    }
    else
    {
        llvm_unreachable("[ResumeInstruction Error] The underlying LLVM instruction is not a resume instruction!");
    }
}

void ResumeInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    llvm::outs() << "resume ";

    llvm::outs() << this->operands[0].first->getValue()->getName();
}

// Catchswitch instruction
CatchswitchInstruction::CatchswitchInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to CATCH_SWITCH
    this->instruction_type = InstructionType::CATCH_SWITCH;

    if (llvm::isa<llvm::CatchSwitchInst>(this->instruction)) { }
    else
        llvm_unreachable("[CatchswitchInstruction Error] The underlying LLVM instruction is not a catchswitch instruction!");
}

void CatchswitchInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Catchreturn instruction
CatchreturnInstruction::CatchreturnInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    if (llvm::isa<llvm::CatchReturnInst>(this->instruction)) { }
    else
        llvm_unreachable("[CatchreturnInstruction Error] The underlying LLVM instruction is not a catchreturn instruction!");
}

void CatchreturnInstruction::printInstruction()
{
    this->printLLVMInstruction();
}

// CleanupReturn instruction
CleanupReturnInstruction::CleanupReturnInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to CLEANUP_RETURN
    this->instruction_type = InstructionType::CLEANUP_RETURN;

    if (llvm::isa<llvm::CleanupReturnInst>(this->instruction)) { }
    else
        llvm_unreachable("[CleanupReturnInstruction Error] The underlying LLVM instruction is not a cleanupreturn instruction!");

}

void CleanupReturnInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Unreachable instruction
UnreachableInstruction::UnreachableInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to UNREACHABLE
    this->instruction_type = InstructionType::UNREACHABLE;

    if (llvm::isa<llvm::UnreachableInst>(this->instruction)) { }
    else
        llvm_unreachable("[UnreachableInstruction Error] The underlying LLVM instruction is not an unreachable instruction!");
}

void UnreachableInstruction::printInstruction()
{
    if (this->hasSourceLineNumber() && this->getSourceLineNumber() != 0)
    {
        llvm::outs() << "[" << this->getSourceLineNumber() << "] ";    
    }

    this->printLLVMInstruction();
}

// Other instruction (currently not supported)
OtherInstruction::OtherInstruction(llvm::Instruction *instruction): BaseInstruction(instruction)
{
    // Set the instruction type to OTHER
    this->instruction_type = InstructionType::OTHER;

    SLIMOperand *result_slim_operand = new SLIMOperand((llvm::Value *) this->instruction);
    this->result = std::make_pair(result_slim_operand, 0);

    for (int i = 0; i < this->instruction->getNumOperands(); i++)
    {
        SLIMOperand *temp_slim_operand = new SLIMOperand(this->instruction->getOperand(i));
        this->operands.push_back(std::make_pair(temp_slim_operand, 0));
    }
}

void OtherInstruction::printInstruction()
{
    llvm::outs() << "Not supported!\n";
}