/**
 * Created by reed on 2016/12/27.
 */
DaQOCloud.factory('homeServices',function(){
    return {
        getRegeoByAjax :function (lng,lat){
            return $.ajax({ type : "GET", url:"http://restapi.amap.com/v3/geocode/regeo?key=b982c4bb4a003d78385a47be87b0276b"
            +"&location="+lng+","+lat+"&radius=1&extensions=all&batch=false&roadlevel=0", async: true});
        },
        markersList: function(){
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/Global/getOnlyProjectInfo?userid=3",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        createMap:function(target,zoom){
            return new AMap.Map(target,{
                zoom : zoom,
                resizeEnable: true,
                layers: [
                    new AMap.TileLayer({defaultType:2,showRoad:true}),//默认图
                    new AMap.TileLayer.Satellite(),//卫星图
                    new AMap.TileLayer.RoadNet()

                ],
                //center: [118.7755100000, 31.8924900000] // 大全集团
                //31.9023600000,118.7784900000  南瑞
                center: [118.7784900000, 31.9023600000,118] // 大全集团
                //center: [121.498586, 31.239637]   // 上海
                //center: [118.83, 31.95]   // 南京市区
                //31.8985885825,118.7819695071
            })
        },
        setFeatures:function (map,list) {
            map.setFeatures(list);
            return map;
        },
        addPlugin:function (map) {
            AMap.plugin(['AMap.ToolBar','AMap.Scale','AMap.OverView','AMap.MapType','AMap.Autocomplete','AMap.PlaceSearch'],
                function(){
                    map.addControl(new AMap.ToolBar());//工具条
                    map.addControl(new AMap.Scale());//比例尺
                    map.addControl(new AMap.OverView({isOpen:false}));//鹰眼
                    autocomplete = new AMap.Autocomplete({city: "", input:"findArea"});
                    autocomplete.on("select", function(e){
                        new AMap.PlaceSearch({city:e.poi.adcode, map:map}).search(e.poi.name)
                    });
                });
            return map;
        },
        initInfoWindowWithAddress:function(result){
            return new AMap.InfoWindow({content: '<P style="font-family: "Microsoft YaHei UI">地址：'+result.regeocode.formatted_address+'</P>',offset:new AMap.Pixel(0, -30),closeWhenClickMap:true});
        },
        initInfoWindow:function(str,projectimg,id){
            var con = [];
            con.push("<img src='resources/img/"+projectimg+"' style='width: 275px;height: 150px;float: left;'>")
            con.push("<span style='font-weight: bold;float: left;word-wrap: break-word;width: 100%'>工程信息: 大全集团</span>")
            con.push("<span style='font-weight: bold;float: left;word-wrap: break-word;width: 275px'>地址: "+str+"</span>")
            con.push("<br/><span style='margin-top: 10px;word-wrap: break-word;float: right'>" +
                "<a style='text-decoration:none;background-color: #f04d22;color:black;border-radius: 5px;padding: 2px 15px 2px 15px;' class='hrefto'>工程链接</a></span>")
            return new AMap.InfoWindow({content: con.join('<br/>'),offset:new AMap.Pixel(0, -20)});
        },
        setMakers:function(info,map,infowindow,notcloseWindow){
            $that = this;
            var address = JSON.parse(info.strPosition.replace(/'/g,'"'))
            return new AMap.Marker({position : new AMap.LngLat(address.lng,address.lat), map : map
            }).on('click',function(e){
                $that.getRegeoByAjax(e.target.getPosition().lng,e.target.getPosition().lat).then(function(result){
                    notcloseWindow = $that.initInfoWindow(result.regeocode.formatted_address,info.strImg,info.intDBPkPjinfoid);
                    notcloseWindow.open(map,e.target.getPosition())})
            }).on('mousedown',function(e){
                if (infowindow != undefined) infowindow.close();
            }).on('mouseup',function(e){
                $that.getRegeoByAjax(e.target.getPosition().lng,e.target.getPosition().lat).then(function(result){
                    infowindow = $that.initInfoWindowWithAddress(result);
                    infowindow.open(map,e.target.getPosition())})
            }).on('mouseover',function(e){
                $that.getRegeoByAjax(e.target.getPosition().lng,e.target.getPosition().lat).then(function(result){
                    infowindow = $that.initInfoWindowWithAddress(result);
                    infowindow.open(map,e.target.getPosition())})
            }).on('mouseout',function(e){
                if (infowindow != undefined) infowindow.close();
            });
        },
        makeMakers:function(map,infowindow,notcloseWindow){
            $that = this;
            this.markersList().then(function(result){
                $(result).each(function (n,e) {
                    $that.setMakers(e,map,infowindow,notcloseWindow);
                });
            });
        }
    }
});