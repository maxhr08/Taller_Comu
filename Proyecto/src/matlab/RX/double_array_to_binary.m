function converted = double_array_to_binary(array)
    
    converted = [];
    row = [];
    
    for i=1:size(array,1)
        for j=1:size(array,2)
            row = strcat(row,num2str(array(i,j)));
        end
        converted = [converted;row];
        row = [];
    end
    
end