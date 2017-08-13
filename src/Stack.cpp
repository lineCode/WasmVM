#include <Stack.h>

Label::Label(bool cont): funcIdx(0), instrOffset(0), cont(cont){
}
Label::Label(const Label& lab){
    funcIdx = lab.funcIdx;
    instrOffset = lab.instrOffset;
    resultTypes = lab.resultTypes;
    cont = lab.cont;
}
Label& Label::operator=(Label lab){
    funcIdx = lab.funcIdx;
    instrOffset = lab.instrOffset;
    resultTypes = lab.resultTypes;
    cont = lab.cont;
    return *this;
}

Frame::Frame():moduleInst(nullptr){
}
Frame::Frame(const Frame& frm){
    locals = frm.locals;
    moduleInst = frm.moduleInst;
}
Frame& Frame::operator=(Frame frm){
    locals = frm.locals;
    moduleInst = frm.moduleInst;
    return *this;
}

Stack::~Stack(){
    for(std::list<StackElem>::iterator stackIt = _stack.begin(); stackIt != _stack.end(); ++stackIt){
        switch(stackIt->type){
            case value:
                delete (Value *)stackIt->data;
            break;
            case label:
                delete (Label *)stackIt->data;
            break;
            case frame:
                delete (Frame *)stackIt->data;
            break;
        }
    }
}
void Stack::push(Value val){
    StackElem elem;
    Value *inst = new Value(val);
    elem.type = value;
    elem.data = inst;
    _stack.push_front(elem);
}
void Stack::push(Label lab){
    StackElem elem;
    Label *inst = new Label(lab);
    elem.type = label;
    elem.data = inst;
    _stack.push_front(elem);
    curLabel = inst;
}
void Stack::push(Frame frm){
    StackElem elem;
    Frame *inst = new Frame(frm);
    elem.type = frame;
    elem.data = inst;
    _stack.push_front(elem);
    curFrame = inst;
}
StackElem &Stack::top(){
    return *(_stack.begin());
}
StackElem Stack::pop(){
    StackElem elem = *(_stack.begin());
    _stack.pop_front();
    if(elem.type == label){
        for(std::list<StackElem>::iterator stackIt = _stack.begin(); stackIt != _stack.end(); ++stackIt){
            if(stackIt->type == label){
                curLabel = (Label *)stackIt->data;
                return elem;
            }
        }
        curLabel = nullptr;
    }else if(elem.type == frame){
        for(std::list<StackElem>::iterator stackIt = _stack.begin(); stackIt != _stack.end(); ++stackIt){
            if(stackIt->type == frame){
                curFrame = (Frame *)stackIt->data;
                return elem;
            }
        }
        curFrame = nullptr;
    }
    return elem;
}
size_t Stack::size(){
    return _stack.size();
}
std::list<StackElem>::iterator Stack::end(){
    return _stack.end();
}
std::list<StackElem>::iterator Stack::begin(){
    return _stack.begin();
}
size_t Stack::valueCount(){
    size_t ret = 0;
    for(std::list<StackElem>::iterator stackIt = _stack.begin(); stackIt != _stack.end(); ++stackIt){
        if(stackIt->type == value){
            ret += 1;
        }
        if(stackIt->type == label){
            break;
        }
    }
    return ret;
}