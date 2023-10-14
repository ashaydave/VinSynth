classdef Delay
    %LONGDELAYCLASS Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        x
        delayTime
        fs
        fb
        bufferSize
        buffer
        writeIndex
    end
    
    methods
        function obj = Delay(delayTime, fs, fb, bufferSize)
            %LONGDELAYCLASS Construct an instance of this class
            %   Detailed explanation goes here
            obj.delayTime = delayTime;
            obj.fs = fs;
            obj.fb = fb;
            obj.bufferSize = bufferSize;
            obj.buffer = zeros(bufferSize, 1);
            obj.writeIndex = 1;
        end
        
         function delayOutput = process(obj, x)
            %METHOD1 Summary of this method goes here
            %   Detailed explanation goes here
            delaySamples = round(obj.delayTime * obj.fs);
            delayOutput = zeros(delaySamples + length(x), 1);

            for n = 1:length(x)
               % Read the delayed sample from the buffer
                readIndex = mod(obj.writeIndex - delaySamples - 1, obj.bufferSize) + 1;
                delayOutput(n) = obj.buffer(readIndex);
                
                % Write the input sample and feedback to the buffer
                obj.buffer(obj.writeIndex) = x(n) + obj.fb * obj.buffer(readIndex);
                obj.writeIndex = mod(obj.writeIndex, obj.bufferSize) + 1;
                
                % If write Index reaches buffer size, wrap back to top
                if (obj.writeIndex == obj.bufferSize)
                    obj.writeIndex = 1;
                end
            end
        end
    end
end

