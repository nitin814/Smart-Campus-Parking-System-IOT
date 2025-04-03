% Cumulative Parking Usage Over Time (Handles NaN)

readChannelID = 2898802;  
readAPIKey = '0977080DQJSIGDX2';

[data, time] = thingSpeakRead(readChannelID, 'Fields', [1,2,3,4], 'NumPoints', 50, 'ReadKey', readAPIKey);


data(isnan(data)) = 0;  

cumulativeUsage = cumsum(sum(data, 2));  % Cumulative sum of occupied spaces

figure;
plot(time, cumulativeUsage, 'b-', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Cumulative Usage');
title('Cumulative Parking Usage Over Time');
grid on;
