# Ray-Audio

This software interprets XML and Lua files for creating audio-oriented UIs. It's Linux-only for now, but any help is appreciated in porting it for other platforms.  
  
* Uses XML language to layout views;
* Lua for creating styles and themes (yet to be implemented);
* OSC for communicating with the audio application (yet to be implemented);
* Native audio input ports, for generating visualizations;
* Native MIDI input ports;
* State control via presets / snapshots (yet to be implemented);
* SDL and Cairo as the graphics engine.
  
A simple, theme-less example:

![](display.gif)

And the XML file example:
```
<?xml version="1.0" encoding="UTF-8"?>
<SimplePanel>
    <Row>
    <Control type="DirTree" fixed="0.2" />
    <Column >
        <Row>
            <Audio type="VU" label="L" channel='0' />
            <Audio type="VU" label="R" channel='1' />
            <Audio type="VU" label="aux-L" channel='2' />
            <Audio type="VU" label="aux-R" channel='3' />
            <Column fixed="0.1">
                <Control type="Knob" channel='48' label="k1" />
                <Control type="Knob" label="k2" />
            </Column>
            <Column fixed="0.1">
                <Control type="Knob" label="k3" />
                <Control type="Knob" label="k4" />
            </Column>
            <Column >
                <Control type="Toggle" label="check this" />
                <Control type="Toggle" label="also this" />
            </Column>
        </Row>
        <Row>
            <Audio type="Oscilloscope" channel='0' label="Scope-L" />
            <Audio type="Oscilloscope" channel='1' label="Scope-R" />
        </Row>
        <Row>
            <Control type="Slider" channel="48" label="Sl-1" />
            <Control type="Slider" channel="49" label="Sl-2" />
            <Control type="Slider" channel="50" label="Sl-3" />
            <Control type="Slider" channel="51" label="Sl-4" />
            <Control type="Slider" channel="52" label="Sl-5" />
            <Control type="Slider" channel="53" label="Sl-6" />
            <Control type="Slider" channel="54" label="Sl-7" />
            <Control type="Slider" channel="55" label="Sl-8" />
            <Control type="Slider" channel="56" label="Sl-9" />
        </Row>
    </Column>
    </Row>
</SimplePanel>
```

