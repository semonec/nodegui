#pragma once

#include <QWidget>
#include <QPushButton>
#include "src/cpp/core/YogaWidget/yogawidget.h"
#include "deps/spdlog/spdlog.h"
#include "napi.h"
#include "napi-thread-safe-callback.hpp"

class NPushButton: public QPushButton, public YogaWidget
{
private:
    std::unique_ptr<ThreadSafeCallback>  emitRef;
public:
    SET_YOGA_WIDGET_Q_PROPERTIES
    using QPushButton::QPushButton; //inherit all constructors of QPushButton
    NPushButton(){
        connect(this, SIGNAL(pressed()),this,SLOT(handleButton()));
    }
    ~NPushButton() {
        this->emitRef.reset();
    };
    Q_OBJECT
private slots:
    void handleButton(){
            emitRef->call([](Napi::Env env, std::vector<napi_value>& args)
            {
                // This will run in main thread and needs to construct the
                // arguments for the call
                args = {  Napi::String::New(env, "clicked"), Napi::String::New(env, "YOLO") };
            });
    }
public:
    void setNodeEmitterEmit( std::unique_ptr<ThreadSafeCallback> &emitterEmit){
        this->emitRef = std::move(emitterEmit); 
    }
};

