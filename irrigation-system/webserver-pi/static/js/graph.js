function render(timeseries, id) {
  colors = ["black", "red", "lime", "blue", "purple"]

  function get_data() {
    data = timeseries['rows']

    for(i in data) {
      for(j in data[i]) {
        v = data[i][j]
        if(!_isDefined(v)) {
          data[i][j] = null
        } else {
          data[i][j] = data[i][j]
        }
      }
    }

    console.log("DATA", data);
    return data;
  }

  function add_circles(line_index) {
    svg.selectAll("myCircles")
      .data(data)
      .enter()
      .append("circle")
        .attr("fill", colors[line_index])
        .attr("stroke", "none")
        .attr("cx", function(d) { return x(d[0]) })
        .attr("cy", function(d) { return y(d[line_index+1]) })
        .attr("r", 3)

    circle = svg.selectAll("circle")
    // circle.filter(function(d) {
    //     // console.log(line_index+1, d[line_index+1], _isDefined(d[line_index+1]))
    //     return _isDefined(d[line_index+1]);
    // })
    // .remove();

  }

  function append_line(line, line_index) {
    // Add the valueline2 path.
    svg.append("path")
        // .datum(data.filter(line.defined()))
        .data([data])
        .attr("class", "line")
        .style("stroke", colors[line_index])
        .attr("d", line);
  }

  function _isDefined(v) {
    isDefined = !isNaN(v) && v != 0 && v !== null;
    // console.log(v, isDefined)
    return isDefined;
  }

  function create_d3_line(line_index) {
    return d3.line()
        .defined(d => _isDefined(d[line_index+1]))
        .x(function(d) { return x(d[0]); })
        .y(function(d) { return y(d[line_index+1]); })
  }

  function create_lines(line_count) {
    for(i=0; i<line_count; i++) {
      var valueline = create_d3_line(i)
      append_line(valueline, i)
      add_circles(i)
    }
  }

  data = get_data()

  line_count = data[0].length - 1

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
  var svg = d3.select("#" + id).append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform",
            "translate(" + margin.left + "," + margin.top + ")");

  // Scale the range of the data
  x.domain(d3.extent(data, function(d) {
    if(d[0] == data[0][0]) {
      console.log("HIGH")
      return timeseries['meta']['high_timestamp_ms'];
    } else {
      console.log("LOW")
      return timeseries['meta']['low_timestamp_ms'];
    }
  }));
  y.domain([0, 100])

  // y.domain([0, d3.max(data, function(d) {
  //   values = d.slice(1)
  //   return Math.max(...values); })]);

  create_lines(line_count)

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
