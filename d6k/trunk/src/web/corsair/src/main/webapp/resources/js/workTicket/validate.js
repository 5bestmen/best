//校验是否为空
function checkNull(obj,showData){
    var time = $(obj).val();
    var regex =/^\d+$/;
    if(isNull(time)||time=="---请选择---"){
        $(showData).html("不能为空！");
        // alert("不能为空!");
        return false;
    }else if(!isNaN(time)){
        $(showData).html("不能为数字！");
        return false;
    }
    $(showData).html("(*)必填");
    return true;
}
//判断字符串时间是否为空
function isNull(timeString){
    if(timeString == null || timeString == ""){
        return true;
    }
    return false;
}

// 比较时间
function compareTime(time1,time2){
    try{
        // 获取时间
        if(time1=="#createTime"){
         var begindate = $(time1).text();
        }else{
         var begindate = $(time1).val();
        }
        var enddate = $(time2).val();
        var begin = changeDate(begindate);
        var end = changeDate(enddate);
        // 获取毫秒
        var beginsecond = begin.getTime();
        var endsecond = end.getTime();
        var days = (endsecond-beginsecond)/(24*3600*1000);
        if(days<0){
            if(time1=="#createTime"){
                $("#endNote").html("创建工作票时间小于终结时间");
                //alert("开始时间小于结束时间，请重新输入！");
            }else{
                $("#endNote").html("开始时间小于结束时间");
                //alert("开始时间小于结束时间，请重新输入！");
            }

            return false;
        }else{
            $("#endNote").html("(*)必填");
            return true;
        }
    } catch (e) {
        console.log(e.message);
    }
}
// 如果你的时间是字符串 yyyy-MM-dd hh:mm:ss ，下面函数可以转成时间
function changeDate(str){
    try{
        var newstr = str.replace(/-/g,'/');
        return new Date(newstr);
    }catch (e){
        console.log("转换时间出错，原因：" + e.message);
    }
}


//校验唯一性
function notRepeat(obj,obj2){
    var map= new Array();
    if(obj.size()>0&& $(obj2).size()>0){
        for(var i =0;i<obj.size();i++){
            data = obj.eq(i).val();
            map.push({i:data});
            map[i].key=data;
        }
        for(var j =0;j<$(obj2).size();j++){
            data = $(obj2).eq(j).val();
            map[j].value=data;
        }
        var hash = {};
        for(var k in map) {
            if (hash[map[k].key]&&hash[map[k].value]){
                alert("内容有重复,请检查！");
                return true;
            }
            hash[map[k].key] = true;
            hash[map[k].value] = true;
        }
        return false;
    }
}

//校验唯一性
function isRepeat(obj){
    var array= new Array();
    if($(obj).size()>0){
        for(var i =0;i<$(obj).size();i++){
            data = $(obj).eq(i).val();
            array.push(data);
        }
        var hash = {};
        for(var k in array) {
            if (hash[array[k]]){
                alert("内容有重复,请检查！");
                return true;
            }
            hash[array[k]] = true;
        }
        return false;
    }
}