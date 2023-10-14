classdef LFO  

    % not sure if I'll get the time to add an LFO or maybe try FM
    properties
        sampleRate
        frequency
        sample
        amplitude
        t = 0;
    end
    
    methods
        
        function obj = LFO(sampleRate)
            obj.sampleRate = sampleRate;
        end
        
        function obj = setParameters(obj, frequency, amplitude)
            obj.frequency = frequency;
            obj.amplitude = amplitude;
        end
        
        function obj = updateOsc(obj)
            obj.sample = obj.amplitude*sin(2*pi*obj.frequency*(obj.t)/obj.sampleRate);
            if (obj.sample<0)
                obj.sample = obj.sample/2.0;
            end
            obj.sample = obj.sample + 1;
            obj.t = obj.t + 1;
        end
        
        function sample = getSample(obj)
            sample = obj.sample;
        end      
    end
end