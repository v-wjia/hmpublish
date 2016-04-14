/**
 * Created by RachelWaffle on 16/4/12.
 */
(function($) {
    Drupal.behaviors.dosomething = {
        attach: function(context, settings) {
            //获得数据
            var queryData = settings.tchart_query;
            //数据库名
            var dbName = settings.tchart_db;

            //根据对应数据库画echarts
            switch (dbName){
                case "streamflow_control_rate":
                    //获得指定div,添加echarts图表的div
                    document.getElementById('js_tchart').innerHTML="<div id=\"mainss\" style=\"width: 820px;height:400px;\"></div>";
                    //echarts代码
                    var myChart = echarts.init(document.getElementById('mainss'));
                    var option = {
                        title: {
                            text: 'ECharts 入门示例'
                        },
                        tooltip: {},
                        legend: {
                            data:['销量']
                        },
                        xAxis: {
                            data: ["衬衫","羊毛衫","雪纺衫","裤子","高跟鞋","袜子"]
                        },
                        yAxis: {},
                        series: [{
                            name: '销量',
                            type: 'bar',
                            data: [5, 20, 36, 10, 10, 20]
                        }]
                    };
                    myChart.setOption(option);

                    break;

            }



        }
    };
})(jQuery);
