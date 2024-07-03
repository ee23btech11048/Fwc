module helloworldfpga(
    output reg redled,      // Red LED for p
    output reg blueled,     // Blue LED for q
    output reg greenled     // Green LED for x
);

    wire clk;                // Clock input
    reg [1:0] count;         // Counter to iterate through values of p and q
    reg [31:0] delay_count;  // Counter to create delay

    // Instantiate the clock source
    qlal4s3b_cell_macro u_qlal4s3b_cell_macro (
        .Sys_Clk0 (clk)
    );

    // Define the truth table logic
    wire p = count[1];
    wire q = count[0];
    wire x = ~p | (p & q);  // x = ~p + pq

    parameter CLOCK_FREQ = 50000000;  // Assume 50 MHz clock
    parameter DELAY_CYCLES = 2 * CLOCK_FREQ;  // 2 seconds delay

    initial begin
        count = 2'b00;        // Start with p=0, q=0
        delay_count = 32'b0;
    end

    always @(posedge clk) begin
        // Create a 2-second delay
        if (delay_count == DELAY_CYCLES - 1) begin
            delay_count <= 32'b0;
            count <= count + 1;  // Move to the next value of p and q
        end else begin
            delay_count <= delay_count + 1;
        end

        // Update LEDs
        redled <= p;
        blueled <= q;
        greenled <= x;
    end
endmodule


