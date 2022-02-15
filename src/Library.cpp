#include <iostream>
#include <cmath>
#include "Library.h"

Vector2 mouse_position;

void drawMovingBar(
    Rectangle rect, float mod, Color color, bool vertical, bool inverted)
{
    DrawRectangle(
        (!vertical && inverted) ? rect.x + rect.width - (mod*rect.width) : rect.x,
        (vertical && !inverted) ? rect.y + rect.height - (mod*rect.height) : rect.y,
        (!vertical) ? mod*rect.width : rect.width, 
        (vertical) ? mod*rect.height : rect.height,
        color
    );
}

void drawLabel(BaseComponent::Label label, Rectangle rect, bool centered = true)
{
    int offX{0};
    if (centered)
        offX = (rect.width/2) - (MeasureText(label.text.c_str(), label.size)/2);
    
    DrawText(
        label.text.c_str(), 
        rect.x + label.offX + offX,
        rect.y - label.size + label.offY,
        label.size,
        label.color
    );
}

void updateValue(Component<Audio::MidiBuffer>* comp)
{
    for (auto& msg : comp->buffer)
        if( ((int)msg[1] == comp->channel) && (comp->midi_value != (int)msg[2]) )
            comp->midi_value = msg[2];   
}

DrawControl Slider = [](Component<Audio::MidiBuffer>* self) 
{
    //those are mock variables, these values should come from outside;
    std::pair<float, float> range{0, 127};   
    bool vertical{true}, inverted{false};

    //update midi value;
    updateValue(self);

    //real drawing part;
    Rectangle bar;
    if (vertical) 
    bar = {
        self->rect.x + (self->rect.width/2) - 10,
        self->rect.y + self->label.size + (spacing * 1.5f),
        20,
        self->rect.height - (spacing * 3) - self->label.size
    };
    else bar = {
        self->rect.x + spacing,
        self->rect.y + self->label.size + (self->rect.height/2) - 10,
        self->rect.width - (spacing * 2),
        20,
    };

    Vector2& mouse = mouse_position;

    if (CheckCollisionPointRec(mouse, bar))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            float newVal{ vertical ? 
                (((bar.y + bar.height) - mouse.y) / bar.height) * range.second
                : ((mouse.x - bar.x) / bar.width) * range.second
            };
            self->midi_value = newVal;
        }
    }
    float ratio = self->midi_value/range.second;
    drawLabel(self->label, bar);
    DrawRectangle(
        bar.x,
        bar.y,
        bar.width,
        bar.height,
        {0, 150, 0, 255}
    );
    drawMovingBar(bar, ratio, {0, 255, 0, 255}, vertical, inverted);
};

DrawAudio VU = [](Component<Audio::AudioBuffer>* self, ...)
{
    bool vertical{false}, inverted{false};
    
    float fixed_width = 20;

    Rectangle bar;
    if (vertical) 
    bar = {
        self->rect.x + (self->rect.width/2) - (fixed_width/2),
        self->rect.y + self->label.size + (spacing * 1.5f),
        fixed_width,
        self->rect.height - (spacing * 3) - self->label.size
    };
    else bar = {
        self->rect.x + spacing,
        self->rect.y + self->label.size + (self->rect.height/2) - (fixed_width/2),
        self->rect.width - (spacing * 2),
        fixed_width,
    };

    float rms{0};
    try {
        for (auto& samp : self->buffer)
            rms += sqrt(std::abs( samp ));
    } catch (...) {
        rms = 0;
    }
    rms = rms/self->buffer.size();

    drawLabel(self->label, bar);
    DrawRectangle(
        bar.x,
        bar.y,
        bar.width,
        bar.height,
        {0, 150, 0, 255}
    );
    drawMovingBar(bar, rms, {0, 255, 0, 255}, vertical, inverted);
};

DrawAudio Oscilloscope = [](Component<Audio::AudioBuffer>* self)
{
    Rectangle cnv{
        self->rect.x + spacing,
        self->rect.y + self->label.size,
        self->rect.width - (spacing*2),
        self->rect.height - self->label.size - spacing,
    };
    float midpoint = cnv.height/2;
    float jump = cnv.width / self->buffer.size();

    DrawRectangle(
        cnv.x,
        cnv.y,
        cnv.width,
        cnv.height,
        {120, 120, 120, 255}
    );
    drawLabel(self->label, cnv, false);
    for (size_t i{0}; i < self->buffer.size()-1; i++)
    {
        float amp{
            ((self->buffer[i]*-1)+1)*(midpoint) + self->rect.y + self->label.size
        };
        float nextamp{
            ((self->buffer[i+1]*-1)+1)*(midpoint) + self->rect.y + self->label.size
        };
        DrawLine((int)i*jump+cnv.x, (int)amp, (int)(i+1)*jump+cnv.x, (int)nextamp, {255, 255, 255, 255} );
    }
};

DrawControl Toggle = [](Component<Audio::MidiBuffer>* self)
{
    float side = 20;
    Rectangle c_box{
        self->rect.x + (self->rect.width/2) - side/2,
        self->rect.y + (self->rect.height/2) - side/2,
        side,
        side
    };
    updateValue(self);

    Vector2& mouse = mouse_position;
    if (CheckCollisionPointRec(mouse, c_box))
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            self->midi_value = !self->midi_value;

    drawLabel(self->label, c_box);
    
    Color col;
    if (self->midi_value > 0) col = {0, 255, 0, 255};
    else col = {0, 120, 0, 255};
    DrawRectangle(
        c_box.x, 
        c_box.y, 
        c_box.width, 
        c_box.height, 
        col
    );
};

DrawControl Knob = [](Component<Audio::MidiBuffer>* self)
{
    std::pair<float, float> range{0, 127}; 

    //update midi value;
    updateValue(self);

    //real drawing part;
    float radius{ (std::min(self->rect.width, self->rect.height)) / 4.0f };
    Vector2 center{
        self->rect.x + (self->rect.width/2),
        self->rect.y + (self->rect.height/2)
    };
    DrawCircle(
        center.x,
        center.y,
        (int)radius,
        {160, 160, 160, 255}
    );

    int state{0};
    if (CheckCollisionPointCircle(mouse_position, center, radius))
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            state = 1;
    if (state) {
        self->midi_value = std::max(
            0.0f, 
            std::min(
                (float)self->midi_value - GetMouseDelta().y,
                    range.second
            )
        );
        if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
            state = 0;
    }

    float angle = (self->midi_value/range.second) * 300 + 120;
    angle = angle * PI / 180;
    Vector2 radians{
        (float)(cos(angle) * radius) + center.x,
        (float)(sin(angle) * radius) + center.y
    };
    DrawLine(center.x, center.y, radians.x, radians.y, {255, 255, 255, 255} );
    drawLabel(self->label, {center.x - radius, center.y - radius, radius*2, radius*2});
};