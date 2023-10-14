classdef Filter

    % From RBJ's EQ Cookbook
    properties
        Fs           
        F0           
        Q           
        gain         
        filterType   
        bufferX1     
        bufferX2    
        bufferY1     
        bufferY2     
    end
    
    methods
        function obj = Filter(Fs, F0, Q, gain, filterType)
            obj.Fs = Fs;
            obj.F0 = F0;
            obj.Q = Q;
            obj.gain = gain;
            obj.filterType = filterType;
            obj.bufferX1 = 0;
            obj.bufferX2 = 0;
            obj.bufferY1 = 0;
            obj.bufferY2 = 0;
        end
        
        function filteredSignal = applyFilter(obj, inputSignal)
            % Filter coefficients
            w0 = 2 * pi * obj.F0 / obj.Fs;
            alpha = sin(w0) / (2 * obj.Q);
            c = cos(w0);
            
            switch obj.filterType
                case 'Lowpass'
                    a0 = (1 - c) / 2;
                    a1 = 1 - c;
                    a2 = (1 - c) / 2;
                    b0 = 1 + alpha;
                    b1 = -2 * c;
                    b2 = 1 - alpha;
                case 'Highpass'
                    a0 = (1 + c) / 2;
                    a1 = -(1 + c);
                    a2 = (1 + c) / 2;
                    b0 = 1 + alpha;
                    b1 = -2 * c;
                    b2 = 1 - alpha;
            end

            % Normalize by a0
            a0 = a0 / b0;
            a1 = a1 / b0;
            a2 = a2 / b0;
            b1 = b1 / b0;
            b2 = b2 / b0;
            
            % Initialize filtered signal
            filteredSignal = zeros(size(inputSignal));
            
            % Process each sample
            for n = 1:length(inputSignal)
                x0 = inputSignal(n);
                
                % Compute filter output
                y0 = a0 * x0 + a1 * obj.bufferX1 + a2 * obj.bufferX2 - b1 * obj.bufferY1 - b2 * obj.bufferY2;
                
                % Update buffers
                obj.bufferX2 = obj.bufferX1;
                obj.bufferX1 = x0;
                obj.bufferY2 = obj.bufferY1;
                obj.bufferY1 = y0;
                
                % Apply gain
                y0 = 10^(obj.gain / 20) * y0;
                
                % Store filtered sample
                filteredSignal(n) = y0;
            end
        end
    end
end
