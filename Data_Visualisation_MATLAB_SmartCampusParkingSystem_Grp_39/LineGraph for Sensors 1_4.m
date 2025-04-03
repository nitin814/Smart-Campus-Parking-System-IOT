readChannelID = 2898802;  
readAPIKey = '0977080DQJSIGDX2';

[data, time] = thingSpeakRead(readChannelID, 'Fields', [1], 'NumPoints', 50, 'ReadKey', readAPIKey);

figure;
plot(time, data(:,1), '-o', 'LineWidth', 2, 'MarkerSize', 5, 'DisplayName', 'Sensor 1');
hold on;
hold off;

datetick('x', 'HH:MM', 'keeplimits'); % Format x-axis as time
xlabel('Time');
ylabel('Value of Sensor');
title('Sensor 1 - Display Value');
grid on;
legend;
set(gca, 'FontSize', 12);