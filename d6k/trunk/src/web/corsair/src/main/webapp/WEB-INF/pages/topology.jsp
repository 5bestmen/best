<%--
  Created by IntelliJ IDEA.
  User: ChengXi
  Date: 2015/9/10
  Time: 10:45
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %>
<html>
<head>
  <meta charset="utf-8">
  <title>${sessionScope.APP_NAME}-设备拓扑</title>
  <link href="resources/css/bootstrap.min.css" rel="stylesheet">
  <link href="resources/css/metisMenu.min.css" rel="stylesheet">
  <link href="resources/css/sb-admin-2.css" rel="stylesheet">
  <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
  <link href="resources/css/style.css" rel="stylesheet" type="text/css">

  <script src="resources/js/jquery.min.js"></script>
  <script src="resources/dist/js/bootstrap-treeview.min.js"></script>
  <script src="resources/js/bootstrap.min.js"></script>
  <script src="resources/js/metisMenu.js"></script>
  <script src="resources/js/sb-admin-2.js"></script>

</head>
<body>

<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>

  <div id="page-wrapper">--%>

    <div class="row">
      <!--div class="col-md-2">
        <div id="tree"></div>
      </div-->
      <!--div class="col-md-10"-->
      <div class="row">

        <div style="width: 100%;background: #fff;text-align: center;">

          <button class="svgZoomBtn btn" zoom="1" style="width: 15%" data-inline="true">放大</button>

          <button class="svgZoomBtn btn" zoom="-1" style="width: 15%" data-inline="true">缩小</button>

          <button class="svgMoveBtn btn" move="left" style="width: 15%" data-inline="true">左移</button>
          <button class="svgMoveBtn btn" move="right" style="width: 15%" data-inline="true">右移</button>
          <button class="svgMoveBtn btn" move="up" style="width: 15%" data-inline="true">上移</button>
          <button class="svgMoveBtn btn" move="down" style="width: 15%" data-inline="true">下移</button>
        </div>
      </div>
      <div class="row">
        <div class="col-md-12">
          <div id="svgdiv" class="panel panel-primary" id="map-0">
            <embed id="svgEmbed" src="/resources/svg_tp/主页面.svg" type="image/svg+xml"
                   onload="initializeSvgDocument()"></embed>
          </div>
        </div>
      </div>
      <!--/div-->
      <div>
      </div>



    </div>
   <%-- </div>
    </div>--%>

    <script>

      var menu = new Object();

      menu.bind = function(event) {			//用于绑定右键菜单的事件
        log(event);
        event.preventDefault();				//阻止浏览器默认菜单弹出
        menu.show('menu', event);
        return false;
      };

      menu.show = function(menuId, event) {	//显示菜单
        this.target = event.target;			//记录触发事件的组件

        $('#' + menuId).menu('show', { left: event.clientX, top: event.clientY });
      };

      menu.view = function() {				//获取对象内部的target属性，并显示文本
        alert($(this.target).text());
      };

      menu.remove = function() {				//删除SVG中触发事件的元素
        $(this.target, svgDocument).remove();
      };

      // 所有量测量
      var measures = {};

      // 所有图形对象  Meas_Ref的ObjectID和图元ID对应关系
      var objects = {};

      var int=self.setInterval(function(){ajaxStart()}, 1000);


      function initializeSvgDocument() {
        svgEmbed = document.getElementById("svgEmbed");
        svgDocument = document.getElementById("svgEmbed").getSVGDocument();

        var viewBoxValues = svgDocument.getElementsByTagName('svg')[0].getAttribute('viewBox').split(' ');
        if (parseFloat(viewBoxValues[2]) > 2500) {
          svgEmbed.width = 2500;
        } else {
          svgEmbed.width = "";
        }


        $.post('/api/svg', {}, function (response) {

          getTree(response);
          //log(response);

        }, 'json');


        //绑定文本元素<text>右键菜单事件属性
        //event为DOM的Event对象，由浏览器自动传入，参考文档：http://www.w3school.com.cn/jsref/dom_obj_event.asp
        //$("img", svgDocument).attr('oncontextmenu', 'parentWindow.menu.bind(event)');

        //$('text', svgDocument).attr('oncontextmenu', 'parentWindow.menu.bind(event)');
        //measures = getAllMeasures();

        getAllObjects();
        //log(objects);
        log(measures);

        for(var m in measures){
          var mes = svgDocument.getElementById(measures[m]);
          if(mes != null) {

            mes.setAttribute('oncontextmenu', 'window.parent.menu.bind(event)');
            mes.oncontextmenu = function (event) {
              menu.bind(event)
            };
          }
        }
      }

      $(".svgMoveBtn").click(function() {

        var viewBoxValues = svgDocument.getElementsByTagName('svg')[0].getAttribute('viewBox').split(' ');

        viewBoxValues[0] = parseFloat(viewBoxValues[0]);
        viewBoxValues[1] = parseFloat(viewBoxValues[1]);

        var move = $(this).attr("move");
        switch (move) {
          case 'left':
            viewBoxValues[0] += 20;
            break;
          case 'right':
            viewBoxValues[0] -= 20;
            break;
          case 'up':
            viewBoxValues[1] += 20;
            break;
          case 'down':
            viewBoxValues[1] -= 20;
            break;
        }

        svgDocument.getElementsByTagName('svg')[0].setAttribute('viewBox', viewBoxValues.join(' '));
      })

      $(".svgZoomBtn").click(function() {

        var zoom = $(this).attr("zoom");
        var cWidth = $("#svgEmbed").width();
        var cHeight = $("#svgEmbed").height();
        if(zoom>0){
          cWidth += cWidth;
          cHeight += cHeight;
        }else if(zoom<0){
          cWidth -= cWidth*0.5;
          cHeight -=  cHeight*0.5;
        }else{
          //原大小
        }

        $("#svgEmbed").width(cWidth);
        $("#svgEmbed").height(cHeight);
      })

      function getData(id) {
        var dataElement = svgDocument.getElementById(id);

        if(dataElement) {
          return get_firstchild(dataElement).innerHTML;
        }

      }

      function get_firstchild(node) {
        var x = node.firstChild;
        while(x && x.nodeType!=1) {
          x = x.nextSibling;
        }
        return x;
      }

      function getLayerElements(Layer) {
        var layerelements = new Array();
        var Element = svgDocument.getElementById(Layer);
        var x = Element.firstElementChild;
        layerelements.push(x);
        for(var i=1; i<Element.childElementCount; i++) {
          layerelements.push(x.nextElementSibling);
          x = x.nextElementSibling;
        }
        return layerelements;
      }

      // 递归函数，遍历parent参数的所有子节点, procfunc参数为遍历中的处理过程
      function getChildNodes(parent, procfunc) {

        /*        var child = parent.childNodes;

         for (var i=0;i<child.length;i++) {
         procfunc(child, parent);
         getChildNodes(child, procfunc);
         }*/

        var child = get_firstchild(parent);
        if (child != null) {
          procfunc(child, parent);
          getChildNodes(child, procfunc);
          while (child.nextElementSibling != null) {
            child = child.nextElementSibling;
            procfunc(child, parent);
            getChildNodes(child, procfunc);

          }
        }
      }

      // 遍历过程发现图形对象保存到map中
      function procGetObject(child, parent) {

        var findPSR = false;
        var findMeas = false;
        var measObjectIDs = new Array();
        if(child.tagName == "metadata") {

          var x = child.childNodes;
          for (var i=0;i<x.length;i++) {
            if(x[i].nodeName == "cge:PSR_Ref") {
              findPSR = true;
            } else if(x[i].nodeName == "cge:Meas_Ref") {
              findMeas = true;
              measObjectIDs.push(x[i].getAttribute("ObjectID"));
            }
          }

          // PSR_Ref和Meas_Ref同时存在即认为图形对象
          if(findPSR && findMeas) {
            for (var i=0;i<measObjectIDs.length;i++) {
              objects[measObjectIDs[i]] = parent.getAttribute("id");
            }
          } else if(!findPSR && findMeas) { // 只有Meas_Ref存在认为量测对象
            for (var i=0;i<measObjectIDs.length;i++) {
              measures[measObjectIDs[i]] = parent.getAttribute("id");
            }
          }
        }

      }


      function modifyStatus(id, data) {
        var dataElement = svgDocument.getElementById(id)
        if(dataElement) {
          var useNode = get_firstchild(dataElement);
          var status = useNode.getAttribute('xlink:href');
          var pre = status.split('@');
          useNode.setAttribute('xlink:href', pre[0] + '@' + data);
        }
      }

      function changeStatus(child) {
        if(child.hasAttribute("xlink:href") && (child.getAttribute("xlink:href") == "#Disconnector:s_刀闸1_0")) {
          child.setAttribute("xlink:href", "#Disconnector:s_刀闸1_1");
        }
      }

      function getAllMeasures() {

        var Element = svgDocument.firstElementChild;

        getChildNodes(Element, log);


        /*        var count = Element.childElementCount;
         log(Element.hasChildNodes());

         for(var i=1; i<count; i++) {
         Element = Element.nextElementSibling;
         log(Element);
         }*/


        //return getLayerElements("TermMeasure_Layer");
        //return getLayerElements("MeasurementValue_Layer");

      }

      // 获取svg中所有图元对象
      function getAllObjects() {

        objects = {};
        measures = {};

        getChildNodes(get_firstchild(svgDocument), procGetObject);
      }

      function getAllBreakers() {

        return getLayerElements("Breaker_Layer");

      }

      /**
       * 改变数字及显示颜色
       * color: 'r', 'red', 'g', 'green', 'y', 'yellow', 'w', 'white'
       * 如果color属性为其他字符串则默认显示为白色
       */
      function modifyData(id, data, color) {
        var dataElement = svgDocument.getElementById(id);
        if(dataElement) {

          var textNode = get_firstchild(dataElement);

          textNode.firstChild.textContent = data;
          //textNode.innerHTML = data;
          //textNode.setAttribute('class', getDataColorClass(color));
        }
      }

      function modifyLine(id, color) {
        var dataElement = svgDocument.getElementById(id);
        if(dataElement) {
          var textNode = dataElement.firstChild.nextSibling;
          textNode.setAttr('class', getLineColorClass(color));
          //textNode.setAttribute('class', getLineColorClass(color));
        }
      }

      /**
       * 通过JQuery修改数字以及颜色
       */
      function modifyDataByJQuery(id, data, color) {
        $('#' + id + ' text', svgDocument).html(data).attr('class', getDataColorClass(color));
      }

      /**
       * 通过JQuery修改线条颜色
       */
      function modifyLineByJQuery(id, color) {
        $('#' + id + ' use', svgDocument).attr('class', getLineColorClass(color));
      }

      /**
       * color: 'r', 'red', 'g', 'green', 'y', 'yellow', 'w', 'white'
       * 如果color属性为其他字符串则默认显示为白色
       */
      function getDataColorClass(color) {
        var colorClass = 'MeasureQ';				//默认使用白色
        if(color == 'r' || color == 'red') {
          colorClass = 'MeasureB';
        } else if(color == 'g' || color == 'green') {
          colorClass = 'MeasureA';
        } else if(color == 'y' || color == 'yellow') {
          colorClass = 'MeasureC';
        }
        return colorClass;
      }

      function getLineColorClass(color) {
        var colorClass = 'GreenLine';				//默认使用绿色
        if(color == 'r' || color == 'red') {
          colorClass = 'RedLine';
        } else if(color == 'g' || color == 'green') {
          colorClass = 'GreenLine';
        } else if(color == 'y' || color == 'yellow') {
          colorClass = 'YellowLine';
        }
        return colorClass;
      }

      function test() {
        //modifyData('10682485_mea', '7.4', 'y');

        for(var m in measures){
          modifyData(measures[m], (Math.random()*100+550).toFixed(), 'y');

        }

        for(var o in objects) {
          modifyStatus(objects[o], (Math.round(Math.random())));
        }

        /*            for(var i=0; i<measures.length; i++) {
         modifyData(measures[i].id, (Math.random()*10).toFixed(1), 'y');
         }*/

        //log(getAllBreakers());
      }


      function log(msg) {
        console.log(msg);
      }

      function ajaxStart() {
        test();
      }

      function getTree(data) {

        $('#tree').treeview({
          data: data,
          onNodeSelected: function(event, data) {

            if(data.text.endWith("svg")) {
              $('#svgEmbed').remove();
              $('<embed id="svgEmbed" src="'+ data.path +'" type="image/svg+xml" onload="initializeSvgDocument()"></embed>').appendTo('#svgdiv');
            }
          },
        });
      }

      String.prototype.endWith=function(endStr){
        var d=this.length-endStr.length;
        return (d>=0&&this.lastIndexOf(endStr)==d)
      }

    </script>

</body>
</html>