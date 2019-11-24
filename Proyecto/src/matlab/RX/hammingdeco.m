function [correctedcode]= hammingdeco(Encoded_Message)
% % Use a [7,4] Hamming code.
% m = 3; n = 2^m-1; k = n-m;
% parmat = hammgen(m); % Produce parity-check matrix.
% trt = syndtable(parmat); % Produce decoding table.
% recd = Encoded_Message; % Suppose this is the received vector.
% syndrome = rem(recd * parmat',2);
% syndrome_de = bi2de(syndrome,'left-msb'); % Convert to decimal.
% disp(['Syndrome = ',num2str(syndrome_de),...
%       ' (decimal), ',num2str(syndrome),' (binary)'])
% corrvect = trt(1+syndrome_de,:); % Correction vector
% % Now compute the corrected codeword.
% correctedcode = rem(corrvect+recd,2);

% Use a [15,11] Hamming code.
    m = 4; n = 2^m-1; k = n-m;

    newH=hammgen(m);
    trt = syndtable(newH); % Produce decoding table.
    recd = Encoded_Message; % Suppose this is the received vector.
    syndrome = rem(recd * newH',2);
    syndrome_de = bi2de(syndrome,'left-msb'); % Convert to decimal.
    disp(['Syndrome = ',num2str(syndrome_de),...
          ' (decimal), ',num2str(syndrome),' (binary)'])
    corrvect = trt(1+syndrome_de,:); % Correction vector
    % Now compute the corrected codeword.
    correctedcode = rem(corrvect+recd,2);
end