classdef BitCrusher < audioPlugin
    % Bit Crusher
    % Author: Ashay Dave
    % email: apd122@miami.edu
    
    % Define tunable properties -> PluginEditor & PluginProcessor
    properties
        bitDepth = 16;
        normalizedFreq = 0.5; % should be between 0 to 1.
    end

    properties (Access = private)
        fs = 48000;
    end
    
    % Map tunable property to plugin parameter
    properties (Constant)
        PluginInterface = audioPluginInterface(...
            audioPluginParameter('bitDepth',...
            'DisplayName','Bit Depth',...
            'Label', 'Bit(s)',...
            'Mapping',{'lin', 1, 16}), ...
            audioPluginParameter('normalizedFreq',...
            'DisplayName','Speed',...
            'Label', 'Hz',...
            'Mapping',{'lin', 0.10, 1.00}));
    end
    
    % Define audio processing
    methods
        function output = process(plugin, input)
            step = 1/2^(plugin.bitDepth);
            phasor = 0;
            quantizedValue = 0;
            output = zeros(size(input,2));

            for channel = 1:size(input,2)
                for n = 1:length(input)
                phasor = phasor + plugin.normalizedFreq;
                    if (phasor >= 1.0)
                    phasor = phasor - 1.0;
                    quantizedValue = step * floor(input(n, channel)/step + 0.5 );
                    end
                output(n, channel) = quantizedValue; %sample and hold
                end
            end           
        end
    end
end
