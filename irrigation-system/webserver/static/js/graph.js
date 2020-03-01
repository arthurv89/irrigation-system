function render(timeseries, id) {
  colors = ["black", "red", "agua", "lime", "blue", "purple"]

  function add_circles(line_index) {
    svg.selectAll("myCircles")
      .data(data)
      .enter()
      .append("circle")
        .attr("fill", colors[line_index])
        .attr("stroke", "none")
        .attr("cx", function(d) { return x(d[0]) })
        .attr("cy", function(d) { return y(d[line_index]) })
        .attr("r", 3)
  }

  function append_line(line, line_index) {
    // Add the valueline2 path.
    svg.append("path")
        .data([data])
        .attr("class", "line")
        .style("stroke", colors[line_index])
        .attr("d", line);
  }

  function create_line(line_index) {
    d3.line()
        .x(function(d) { return x(d[0]); })
        .y(function(d) { return y(d[line_index]); })
  }

  function add_lines() {
    // define the 1st line
    var valueline = create_line(1);
    var valueline2 = create_line(2);
    var valueline3 = create_line(3);

    // Add the valueline path.
    append_line(valueline, 1)
    append_line(valueline2, 2)
    append_line(valueline3, 3)

    // Add the line
    add_circles(1)
    add_circles(2)
    add_circles(3)
  }

  function max_y(d) {
    return Math.max(d[1], d[2], d[3])
  }

    data = timeseries['rows']
    console.log(data)

    for(i in data) {
      data[i][1] *= 1000
    }

    // set the dimensions and margins of the graph
    var margin = {top: 20, right: 20, bottom: 30, left: 50},
        width = 960 - margin.left - margin.right,
        height = 500 - margin.top - margin.bottom;

    // set the ranges
    var x = d3.scaleTime().range([0, width]);
    var y = d3.scaleLinear().range([height, 0]);


    // append the svg obgect to the body of the page
    // appends a 'group' element to 'svg'
    // moves the 'group' element to the top left margin
    var svg = d3.select("body").append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
        .append("g")
        .attr("transform",
              "translate(" + margin.left + "," + margin.top + ")");

    // Scale the range of the data
    x.domain(d3.extent(data, function(d) { return d[0]; }));
    y.domain([0, d3.max(data, function(d) { return max_y(d); })]);

    add_lines()

    // Add the X Axis
    svg.append("g")
        .attr("class", "axis")
        .attr("transform", "translate(0," + height + ")")
        .call(d3.axisBottom(x)
                .tickFormat(d3.timeFormat("%H:00")))

    // Add the Y Axis
    svg.append("g")
        .call(d3.axisLeft(y));
}
