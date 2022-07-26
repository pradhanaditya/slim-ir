#include "Operand.h"

// Operand is not a address-taken local variable
SLIMOperand::SLIMOperand(llvm::Value *value)
{
    this->value = value;
    this->is_global_or_address_taken = false;
    this->is_pointer_variable = false;

    if (value != nullptr)
    {
        /*
            Since every global and address taken variable in the LLVM IR (after mem2reg pass) is 
            a pointer, we distinguish pointers in the source program from the pointers in LLVM by
            checking whether a LLVM pointer is double pointer or not. 
            
            But we have to be careful regarding this because there might be some instances in which we 
            can interpret the operand incorrectly. For example, a GetElementPtr (GEP) operand is always 
            a pointer but if we try to check this by taking the operand (of GEPOperand) then we may get 
            invalid results because that operand may or may not be a pointer.
        */

        if (value->getType()->getNumContainedTypes() > 0 && value->getType()->getContainedType(0)->isPointerTy())
        {
            this->is_pointer_variable = true;
        }
    }

    // Set the operand type
    if (this->value != nullptr)
    {
        // First check if the operand has a name, which won't be the case when the operand is a 
        // GetElementPtr (GEP) operand for example, because in this case we have to extract the relevant
        // operand and indices from the GEP operand.    
        if (this->value->hasName())
        {
            this->operand_type = OperandType::VARIABLE;
        }
        else if (llvm::isa<llvm::GEPOperator>(this->value))
        {
            this->operand_type = OperandType::GEP_OPERATOR;
        }
        else if (llvm::isa<llvm::AddrSpaceCastOperator>(this->value))
        {
            this->operand_type =OperandType::ADDR_SPACE_CAST_OPERATOR;
        }
        else if (llvm::isa<llvm::BitCastOperator>(this->value))
        {
            this->operand_type = OperandType::BITCAST_OPERATOR;
        }
        else if (llvm::isa<llvm::PtrToIntOperator>(this->value))
        {
            this->operand_type = OperandType::PTR_TO_INT_OPERATOR;
        }
        else if (llvm::isa<llvm::ZExtOperator>(this->value))
        {
            this->operand_type = OperandType::ZEXT_OPERATOR;
        }
        else if (llvm::isa<llvm::FPMathOperator>(this->value))
        {
            this->operand_type = OperandType::FP_MATH_OPERATOR;
        }
        else if (llvm::isa<llvm::Constant>(this->value))
        {
            if (llvm::isa<llvm::BlockAddress>(this->value))
            {
                this->operand_type = OperandType::BLOCK_ADDRESS;
            }
            else if (llvm::isa<llvm::ConstantAggregate>(this->value))
            {
                this->operand_type = OperandType::CONSTANT_AGGREGATE;
            }
            else if (llvm::isa<llvm::ConstantDataSequential>(this->value))
            {
                this->operand_type = OperandType::CONSTANT_DATA_SEQUENTIAL;
            }
            else if (llvm::isa<llvm::ConstantPointerNull>(this->value))
            {
                this->operand_type = OperandType::CONSTANT_POINTER_NULL;
            }
            else if (llvm::isa<llvm::ConstantTokenNone>(this->value))
            {
                this->operand_type = OperandType::CONSTANT_TOKEN_NONE;
            }
            else if (llvm::isa<llvm::UndefValue>(this->value))
            {
                this->operand_type = OperandType::UNDEF_VALUE;
            }
            else if (llvm::isa<llvm::ConstantInt>(this->value))
            {
                this->operand_type = OperandType::CONSTANT_INT;
            }
            else if (llvm::isa<llvm::ConstantFP>(this->value))
            {
                this->operand_type = OperandType::CONSTANT_FP;
            }
            else if (llvm::isa<llvm::DSOLocalEquivalent>(this->value))
            {
                this->operand_type = OperandType::DSO_LOCAL_EQUIVALENT;
            }
            else if (llvm::isa<llvm::GlobalValue>(this->value))
            {
                this->operand_type = OperandType::GLOBAL_VALUE;
            }
            else if (llvm::isa<llvm::NoCFIValue>(this->value))
            {
                this->operand_type = OperandType::NO_CFI_VALUE;
            }
            else
            {
                this->operand_type = OperandType::NOT_SUPPORTED_OPERAND;
            }
        }
        else
        {
            this->operand_type = OperandType::NOT_SUPPORTED_OPERAND;
        }
    }
}

// Operand may or may not be a address-taken local or global variable
SLIMOperand::SLIMOperand(llvm::Value *value, bool is_global_or_address_taken)
{
    this->value = value;
    this->is_global_or_address_taken = is_global_or_address_taken;

    this->is_pointer_variable = false;

    if (value != nullptr)
    {
        // Same argument (described in the above constructor)
        if (value->getType()->getNumContainedTypes() > 0 && value->getType()->getContainedType(0)->isPointerTy())
        {
            this->is_pointer_variable = true;
        }
    }
}

// Returns the operand type
OperandType SLIMOperand::getOperandType()
{
    return this->operand_type;
}

// Returns true if the operand is a global variable or a address-taken local variable
bool SLIMOperand::isGlobalOrAddressTaken()
{
    return this->is_global_or_address_taken;
}

// Returns true if the operand is a pointer variable
bool SLIMOperand::isPointerVariable()
{
    return this->is_pointer_variable;
}

// Sets the is_pointer_variable to true
void SLIMOperand::setIsPointerVariable()
{
    this->is_pointer_variable = true;
}

// Sets the is_pointer_variable to false
void SLIMOperand::unsetIsPointerVariable()
{
    this->is_pointer_variable = false;
}

// Returns the pointer to the corresponding llvm::Value object
llvm::Value* SLIMOperand::getValue()
{
    return this->value;
}

// Internal function to be used only in case of print related tasks
std::string SLIMOperand::_getOperandName()
{
    llvm::Value *operand = this->getValue();

    // This will hold the string value of the operand
    std::string operand_name;

    // The string in this stream will be flushed into the operand_name
    llvm::raw_string_ostream stream(operand_name);
    
    // First check if the operand has a name, which won't be the case when the operand is a 
    // GetElementPtr (GEP) operand for example, because in this case we have to extract the relevant
    // operand and indices from the GEP operand 
    if (operand->hasName())
    {
        stream << operand->getName();
    }
    else if (llvm::isa<llvm::GEPOperator>(operand))
    {
        // Cast the operand to llvm::GEPOperator
        llvm::GEPOperator *gep_operator = llvm::cast<llvm::GEPOperator>(operand);

        // Get the variable operand (which is the first operand)
        llvm::Value *gep_operand = gep_operator->getOperand(0);

        // Print the variable name
        stream << gep_operand->getName();

        // Print the indices
        for (int i = 1; i < gep_operator->getNumOperands(); i++)
        {
            llvm::Value *index_val = gep_operator->getOperand(i);

            // Check if the index is constant
            if (llvm::isa<llvm::Constant>(index_val))
            {
                if (llvm::isa<llvm::ConstantInt>(index_val))
                {
                    llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(index_val);

                    // Print the constant integer
                    stream << "[" << constant_int->getSExtValue() << "]";
                }
                // If the indices are constant, they must be integers
                else
                {
                    llvm_unreachable("[GetElementPtrOperator Error] The index is a constant but not an integer constant!");
                }
            }
            else
            {
                // The index is stored in a variable or SSA register
                stream << "[" << index_val->getName() << "]";
            }
        }
    }
    else if (llvm::isa<llvm::AddrSpaceCastOperator>(operand))
    {
        llvm::cast<llvm::AddrSpaceCastOperator>(operand)->print(stream);
    }
    // else if (llvm::isa<llvm::AddrSpaceCastInst>(operand))
    // {
    //     llvm::cast<llvm::AddrSpaceCastInst>(operand)->print(stream);
    // }
    else if (llvm::isa<llvm::BitCastOperator>(operand))
    {
        llvm::BitCastOperator *bitcast_operator = llvm::cast<llvm::BitCastOperator>(operand);

        bitcast_operator->print(stream);
    }
    else if (llvm::isa<llvm::PtrToIntOperator>(operand))
    {
        llvm::cast<llvm::PtrToIntOperator>(operand)->print(stream);
    }
    else if (llvm::isa<llvm::ZExtOperator>(operand))
    {
        llvm::cast<llvm::ZExtOperator>(operand)->print(stream);
    }
    else if (llvm::isa<llvm::FPMathOperator>(operand))
    {
        llvm::cast<llvm::FPMathOperator>(operand)->print(stream);
    }
    else if (llvm::isa<llvm::Constant>(operand))
    {
        if (llvm::isa<llvm::BlockAddress>(operand))
        {
            llvm::BlockAddress *block_address = llvm::cast<llvm::BlockAddress>(operand);

            stream << block_address->getBasicBlock()->getName();
        }
        else if (llvm::isa<llvm::ConstantAggregate>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support constant aggregate!");
        }
        else if (llvm::isa<llvm::ConstantDataSequential>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support constant data sequential!");
        }
        else if (llvm::isa<llvm::ConstantPointerNull>(operand))
        {
            llvm::ConstantPointerNull *const_pointer_null = llvm::cast<llvm::ConstantPointerNull>(operand);

            if (const_pointer_null)
                stream << const_pointer_null->getName() << " ";
            
            stream << "nullptr";
        }
        else if (llvm::isa<llvm::ConstantTokenNone>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support constant token none!");
        }
        else if (llvm::isa<llvm::UndefValue>(operand))
        {
            stream << "undef";
        }
        else if (llvm::isa<llvm::ConstantInt>(operand))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand);

            stream << constant_int->getSExtValue();
        }
        else if (llvm::isa<llvm::ConstantFP>(operand))
        {
            llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand);

            stream << constant_float->getValueAPF().convertToFloat();
        }
        else if (llvm::isa<llvm::DSOLocalEquivalent>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support DSO local equivalent!");
        }
        else if (llvm::isa<llvm::GlobalValue>(operand))
        {
            stream << llvm::cast<llvm::GlobalValue>(operand)->getName();
        }
        else if (llvm::isa<llvm::NoCFIValue>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support NoCFIValue!");
        }
        else
        {
            llvm_unreachable("[SLIMOperand Error] Unexpected constant!");
        }
    }
    else
    {
        //stream << operand->getName();
        llvm_unreachable("[SLIMOperand Error] Unexpected operand!");
    }

    std::string result = std::string(stream.str());
    
    //stream.flush();
    
    return result;
}

// Print the SLIM operand
void SLIMOperand::printOperand(llvm::raw_ostream &stream)
{
    stream << llvm::StringRef(this->_getOperandName());

    return ;
}

// --------------- APIs for the Legacy SLIM ---------------
    
// Returns the name of the operand
llvm::StringRef SLIMOperand::getName()
{
    std::string *operand_name = new std::string(this->_getOperandName());
    return llvm::StringRef(*operand_name);
}

// --------------------------------------------------------

// Methods of the OperandRepository namespace

namespace OperandRepository
{
    // Check whether a SLIMOperand object corresponds to a global or a address-taken local variable or not
    std::map<llvm::Value *, SLIMOperand *> value_to_slim_operand;
};

SLIMOperand * OperandRepository::getSLIMOperand(llvm::Value *value)
{
    if (OperandRepository::value_to_slim_operand.find(value) != OperandRepository::value_to_slim_operand.end())
    {
        return OperandRepository::value_to_slim_operand[value];
    }

    return nullptr;
}

void OperandRepository::setSLIMOperand(llvm::Value *value, SLIMOperand *slim_operand)
{
    OperandRepository::value_to_slim_operand[value] = slim_operand;
}