function render(datapoints, id) {
  // Just a normal responce from the server
  var response = {
    "status": "Ok",
    "data": datapoints
  };

  // Parse the response
  var chart = new Vue({
    el: '#' + id,
    data: function() {
      return {
        layout: {
          width: 800,
          height: 400,
          margin: {
            left: 50,
            top: 50,
            right: 50,
            bottom: 50
          }
        },
        plot: {
          points: []
        }
      }
    },

    // Computed functions
    computed: {

      // Return dimensions of SVG chart
      svgViewBox: function() {
        return '0 0 ' + (this.layout.width + this.layout.margin.left + this.layout.margin.right) + ' ' + (this.layout.height + this.layout.margin.top + this.layout.margin.bottom);
      },

      // Stage
      stageTransform: function() {
        return {
          'transform': 'translate(' + this.layout.margin.left + 'px,' + this.layout.margin.top + 'px)'
        }
      }
    },

    // Initialisation
    mounted: function() {

      // Update plot
      this.update();
    },

    // Methods
    methods: {
      // Update elements in chart
      update: function() {
        // Internal variables
        var _w = this.layout.width;
        var _h = this.layout.height;

        // Compute scale
        this.plot.scale = {
          x: d3.scaleTime().range([0, _w]),
          y: d3.scaleLinear().range([_h, 0])
        };
        var scale = this.plot.scale;
        //
        // // Generate area
        // this.plot.area = d3.area()
        //   .x(function(d) { return scale.x(d.date); })
        //   .y1(function(d) { return scale.y(d.moisture); });

        // Generate line
        this.plot.line = d3.line()
          .defined(function (d) { return d[1] !== null; })
          .x(function(d) { return scale.x(d.date); })
          .y(function(d) { return scale.y(d.moisture); });

        // Push individual points into data
        var _d = response.data;
        for (let i in _d) {
          console.log(i, _d)
          this.plot.points.push({
            date: new Date(_d[i][0]),  // Clean up dates with trailing GMT offsets
            moisture: _d[i][1]
          })
        }

        // Set extend of data
        this.plot.scale.x.domain(d3.extent(this.plot.points, function(d) { return d.date; }));
        this.plot.scale.y.domain([0, d3.max(this.plot.points, function(d) { return d.moisture; })]);
        // this.plot.area.y0(this.plot.scale.y(0));

        // Draw axes
        d3.select(this.$refs.xAxis)
        .attr('transform', 'translate(0,' + this.layout.height + ')')
        .call(
          d3.axisBottom(scale.x)
          .ticks(7)
          .tickFormat(d3.timeFormat("%-H:%M"))
        );
        d3.select(this.$refs.yAxis)
        .call(
          d3.axisLeft(scale.y)
        );

        // Draw area
        // var $area = d3.select(this.$refs.area);
        // $area
        //   .datum(this.plot.points)
        //   .attr('d', this.plot.area)
        //   .attr('fill', 'none');

        // Draw line
        var $line = d3.select(this.$refs.line);
        $line
          .data([this.plot.points])
          .attr('d', this.plot.line);

        // Draw points
        var $g = d3.select(this.$refs.points);
        $g.selectAll('circle.point').data(this.plot.points)
          .enter()
          .append('circle')
            .attr('r', 5)
            .attr('class', 'point')
            .attr('cx', function(d) { return scale.x(d.date); })
            .attr('cy', function(d) { return scale.y(d.moisture); });

      }
    }
  });
}
