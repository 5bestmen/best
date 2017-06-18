<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html lang="zh_CN">
<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">


		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

		<%--<script src="dist/js/html5shiv.min.js"></script>--%>
		<%--<script src="dist/js/respond.min.js"></script>--%>

</head>
<body>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>
		<script type="application/javascript">
			$(function(){
				$("tbody tr:even").css("background-color","rgb(31, 86, 134)");
				$("tbody tr:even").children().css("color","white");
				/*$("tbody tr:even").children().eq(2).children(0).children(0).attr("src","/resources/img/Edit_W.png");
				$("tbody tr:even").children().eq(2).children(0).children(0).attr("src","/resources/img/Eye Open_W.png");*/


				$("tbody tr:odd").css("background","#82accb");
				/*$("tbody tr:odd").children().eq(2).children(0).children(0).attr("src","/resources/img/Edit_B.png");
				$("tbody tr:odd").children().eq(2).children(0).children(0).attr("src","/resources/img/Eye Open_B.png");*/
				var num = $("tbody tr:odd").children().length;
				for(var i=1;i < num;i++){
					var count =7*i-1;
					$("tbody tr:even").children().eq(count).children().eq(0).children().eq(0).attr("src","/resources/img/Edit_W.png");
					$("tbody tr:odd").children().eq(count).children().eq(0).children().eq(0).attr("src","/resources/img/Edit_B.png");
					$("tbody tr:even").children().eq(count).children().eq(1).children().eq(0).attr("src","/resources/img/Eye Open_W.png");
					$("tbody tr:odd").children().eq(count).children().eq(1).children().eq(0).attr("src","/resources/img/Eye Open_B.png");
				}

			});

		</script>
        <script type="text/javascript">
			function checkAll(checkTopId, checkItemName){
				var objs = document.all(checkItemName);
				var check = document.getElementById(checkTopId).checked;
				if (objs != null) {
					if(objs.length == undefined){
						if(check == true){
							objs.checked = true;
						}else{
							objs.checked = false;
						}
					}else{
						for(var i=0; i<objs.length; i++) {
							if(check == true){
								objs[i].checked = true;
							}else{
								objs[i].checked = false;
							}
						}
					}
				}
			}

			function check(checkTopId, checkItemName){
				document.getElementById(checkTopId).checked = true;
				var objs = document.all(checkItemName);
				if (objs != null) {
					if(objs.length == undefined){
						if(objs.checked == false)
						{
							document.getElementById(checkTopId).checked = false;
						}
					}else {
						for(var i=0; i<objs.length; i++) {
							if(objs[i].checked == false)
							{
								document.getElementById(checkTopId).checked = false;
							}
						}
					}
				}
			}

			function actualTimeMailReceive(flag,startFlag){
//				if("1"==startFlag){
//					$("#clockSend").hide();
//					$("#cancelClockSend").show();
//				}else if("0"==startFlag){
//					$("#clockSend").show();
//					$("#cancelClockSend").hide();
//				}
				var obj = document.getElementsByName('checkbox1');
				var obj_mailAddress = document.getElementsByName('mailAddress');
				var checkNum = 0;
				if(obj){
					var mail_address =null;
					var mail_id = null;
					for(var   i=0;   i <obj.length;   i++){
						if(obj[i].checked   ==   true){
							checkNum++;
							document.getElementById("mail_Address").value +=obj_mailAddress[i].value+",";
							document.getElementById("mail_id").value +=obj[i].value+"-";
						}
					}
				}
				if(checkNum == 0){
					alert( "请选择接收邮件附件人 ");
				}else{
					mail_address =  document.getElementById("mail_Address").value;
					mail_id =  document.getElementById("mail_id").value;
					mail_address=mail_address.substring(0,mail_address.length-1);
					mail_id=mail_id.substring(0,mail_id.length-1);
					// if(window.confirm("纭畾瑕佷慨鏀归�涓殑璁板綍鍚�")){
					if("0"==flag){
						window.location.href="actualTimeMailReceive?mailAddress="+mail_address+"&param="+startFlag+"&mail_id="+mail_id;
					}else if("1"==flag){
						window.location.href="clockingMailReceive?mailAddress="+mail_address+"&param="+startFlag+"&mail_id="+mail_id;
					}
//					else if("2"==flag){
//						window.location.href="clockingMailReceive?mailAddress="+mail_address+"&param="+startFlag+"&mail_id="+mail_id;
//					}
					//}
				}
			}
			function mailModify(id,flag){
				var obj = document.getElementsByName('checkbox1');
				var checkNum = 0;
				if(obj){
					var mail_id ;
					for(var   i=0;   i <obj.length;   i++){
						if(obj[i].checked   ==   true){
							checkNum++;
							document.getElementById("mail_id").value =obj[i].value;
						}
					}
				}
				if(checkNum == 0){
					alert( "请选择收件人 ");
				}else if(checkNum > 1){
					alert( "请选择一个收件人 ");
				}else{
					mail_id =  document.getElementById("mail_id").value;
					// if(window.confirm("纭畾瑕佷慨鏀归�涓殑璁板綍鍚�")){
					window.location.href="clockingMailReceive?id="+mail_id+"&flag=1";
					//}
				}
			}
        </script>
		<script type="text/javascript">
			$(function(){
				var GET = $.urlGet(); //获取URL的Get参数
                var fg = GET['param_flag']; //取得param_flag的值
				var mail_person_id = GET['mail_id']; //取得mail_id的值
				if("0"==fg){
					ischeckecMethod(mail_person_id,fg);
				}
				else if("1"==fg){
					ischeckecMethod(mail_person_id,fg);
				}
//				else if("2"==fg){
//					ischeckecMethod(mail_person_id,fg);
//				}
				function ischeckecMethod(mail_person_id,fg){
					if("0"==fg){
						if(null!=mail_person_id){
							var check_v = mail_person_id.split("-");
							if(check_v.length>0){
								for(var i=0;i<check_v.length;i++){
									$("#"+check_v[i]).attr("checked",true);
								}
							}
						}
					}
					else if("1"==fg){
						$("#clockSend").hide();
						$("#cancelClockSend").show();
						if(null!=mail_person_id){
							var check_v = mail_person_id.split("-");
							if(check_v.length>0){
								for(var i=0;i<check_v.length;i++){
									$("#"+check_v[i]).attr("checked",true);
								}
							}
						}
					}
//					else if("2"==fg){
//						$("#clockSend").show();
//						$("#cancelClockSend").hide();
//						if(null!=mail_person_id){
//							var check_v = mail_person_id.split("-");
//							if(check_v.length>0){
//								for(var i=0;i<check_v.length;i++){
//									$("#"+check_v[i]).attr("checked",false);
//								}
//							}
//						}
//					}
				}
			});
		</script>
		<script type="text/javascript">
			(function($) {
				$.extend({
					urlGet:function()
					{
						var aQuery = window.location.href.split("?");  //取得Get参数
						var aGET = new Array();
						if(aQuery.length > 1)
						{
							var aBuf = aQuery[1].split("&");
							for(var i=0, iLoop = aBuf.length; i<iLoop; i++)
							{
								var aTmp = aBuf[i].split("=");  //分离key与Value
								aGET[aTmp[0]] = aTmp[1];
							}
						}
						return aGET;
					}
				})
			})(jQuery);
		</script>
		<script type="text/javascript">
			function up() {
				if($("table input[name='checkbox1']:checked").length==0){
					alert("请选择一条记录");
					return;
				}
				if($("table input[name='checkbox1']:checked").length>1){
					alert("请选择一条记录");
					return;
				}
				$.each($("table input[name='checkbox1']:checked"), function() {
					var onthis = $(this).parent().parent();
					var getUp = onthis.prev();
					if ($(getUp).has("input[name='checkbox1']").size() == 0) {
						alert("已是最顶层！");
						return false;
					}
					$(onthis).after(getUp);
				});
			}
			//下移
			function down() {
				if($("table input[name='checkbox1']:checked").length==0){alert("请选择一条记录");return ;}
				if($("table input[name='checkbox1']:checked").length>1){
					alert("请选择一条记录");
					return;
				}
				var last_checked = $("table input[name='checkbox1']:checked").last();//获取选中checkbox中的最后一个
				var last_input = $(last_checked).parent().parent();//获取选中Checkbox中最后一个的tr
				var last_next = $(last_input).next();//获取该tr的下一个同胞元素
				if ($(last_next).has("input").size() != 0) {
					$.each($("table input[name='checkbox1']:checked"), function() {
						var onthis = $(this).parent().parent(); //获取到tr行
						var getdown = onthis.next();//得到选中tr的下一行
						var next_checkbox = $(getdown).children().children();
						while ($(next_checkbox).attr("checked") == "checked") {//（这个while循环起到固定移动的作用）如果下一个checkbox是选中的那么继续寻找下一个
							getdown = getdown.next();
							next_checkbox
						}
						if ($(getdown).has("input").size() == 0) {
							alert("已是最底层！");
							return false;
						} else {
							$(getdown).after(onthis); //替换
						}
					});
				} else {
					alert("已是最底层");
					return false;
				}

			}
			var upFirst =function (){
				if($("table input[name='checkbox1']:checked").length==0){alert("请选择一条记录");return ;}
				if($("table input[name='checkbox1']:checked").length>1){
					alert("请选择一条记录");
					return;
				}
				var onthis = $("table input[name='checkbox1']:checked").parent().parent();
				var getUp = onthis.prev();
				if ($(onthis).has("input[name='checkbox1']").size() == 0) {
					alert("已是最顶层！");
					return false;
				}else{
					if($(getUp).has("input[name='checkbox1']").size() > 0){
//						var tr_html=$("#mytable").find("tr").first().next().innerText;
//						var onthis_html = onthis.innerText;
//						if(tr_html==onthis_html){
//							alert("已是最顶层！");
//						}else{
							$("#mytable").find("tr").first().next().before(onthis);
//						}
					}else{
						alert("已是最顶层！");
					}
					//$(onthis).after(getUp);
				}
			}

			var downLast =function (){
				if($("table input[name='checkbox1']:checked").length==0){alert("请选择一条记录");return ;}
				if($("table input[name='checkbox1']:checked").length>1){
					alert("请选择一条记录");
					return;
				}
				var onthis = $("table input[name='checkbox1']:checked").parent().parent();
				var getUp = onthis.next();
				if ($(onthis).has("input").size() == 0) {
					alert("已是最底层！");
					return false;
				}else{
					if($(getUp).has("input").size() > 0){
						$("#mytable").find("tr").last().after(onthis);
					}else{
						alert("已是最底层！");
					}
				}
			}
			var delResource=function(obj){
				if(confirm('是否确定删除这条记录？')){
					window.location.href="clockingMailReceive?id="+obj;
				}
			}
		</script>
		<%--<div id="wrapper">
			<jsp:include page="meun.jsp"/>
				<div id="page-wrapper">--%>
					<div class="row">
						<div class="col-lg-12">
							<h3 class="page-header">邮件接收人列表</h3>
						</div>
						<!-- /.col-lg-12 -->
					</div>
					<!-- /.row -->
                   <form:form cssClass="form-inline" action="mailReceiveList" method="post" commandName="mailReceiveListForm">
					   <form:hidden property="deleteIds" path="deleteIds"/>
					   <input type="hidden" id="mail_Address"/>
					   <input type="hidden" id="mail_id"/>
					   <form:input type="submit" property="del" value="批量删除" cssStyle="display:none" path="del"/>
					   <div class="form-group">
						   <input class="form-control" placeholder="人名" name="mailNameQry" autofocus/>
					   </div>
					   <div class="form-group">
						   <input class="form-control" placeholder="邮箱地址" name="mailAddressQry"/>
					   </div>
					   <button type="submit" class="btn btn-primary">
						   <i class="glyphicon glyphicon-search"></i>
						   查询收件人
					   </button>
					    <a class="btn btn-primary"  onclick="actualTimeMailReceive('0','0');">
						   <i class="glyphicon glyphicon-plus"></i>
						    实时发送
					   </a>
						<%--<a class="btn btn-primary"  onclick="actualTimeMailReceive('1','1');" id="clockSend">--%>
							<%--<i class="glyphicon glyphicon-plus"></i>--%>
							<%--定时发送--%>
						<%--</a>--%>
					    <%--<a class="btn btn-primary"  onclick="actualTimeMailReceive('2','2');" style="display: none" id="cancelClockSend">--%>
						   <%--<i class="glyphicon glyphicon-plus"></i>--%>
						   <%--取消定时发送--%>
					   <%--</a>--%>
					   <a class="btn btn-primary" href="mailReceiveModify">
						   <i class="glyphicon glyphicon-plus"></i>
						   新增邮件接收人
					   </a>
					   	<div style="margin:10px 0;"></div>
						<div class="row">
							<div class="col-lg-12 col-md-12">
								<div class="table-responsive">
									<table class="table table-bordered table-striped" id="mytable">
										<thead>
										<tr id="first">
											<th style="width: 5%">
												<input id="checkboxtop"	type="checkbox"  title="选中/取消选中" value="" onclick="checkAll('checkboxtop','checkbox1');" />
											</th>
											<th style="width: 10%">
												接收者ID
											</th>
											<th style="width: 10%">
												人名
											</th>
											<th style="width: 10%">
												职务
											</th>
											<th style="width: 10%">
												号码
											</th>
											<th style="width: 15%">
												邮箱地址
											</th>
											<th style="width: 20%">
												操作
											</th>
										</tr>
										</thead>
										<tbody>
										<c:if test="${not empty mailReceiveList}">
											<c:forEach items="${mailReceiveList}" var="mailReceive">
												<tr>
													<td>
														<input name="checkbox1" id="${mailReceive.id}" type="checkbox"  value="${mailReceive.id}" onclick="check('checkboxtop','checkbox1');" />
													</td>
													<td>
														<c:out value="${mailReceive.id}"/>
													</td>
													<td>
														<c:out value="${mailReceive.receiverName}"/>
													</td>
													<td>
														<c:out value="${mailReceive.receiverJob}"/>
													</td>
													<td>
														<c:out value="${mailReceive.phone}"/>
													</td>
													<td>
														<c:out value="${mailReceive.mailAddress}"/>
														<input type="hidden" name="mailAddress" value="${mailReceive.mailAddress}"/>
													</td>
													<td align="left">
														<a  href="mailReceiveModify?id=${mailReceive.id }&flag=1">
															<%--<i class="glyphicon glyphicon-edit"></i>class="btn btn-primary"
															编辑--%>
															<img />
														</a>&nbsp;
														<a href="mailReceiveModify?id=${mailReceive.id }&flag=0">
															<%--<i class="glyphicon glyphicon-edit"></i>class="btn btn-primary"
															查看--%>

																<img />
														</a>&nbsp;
														<a onclick="delResource(${mailReceive.id })">
															<%--<i class="glyphicon glyphicon-remove"></i>class="btn btn-warning"
															删除--%>
																<img src="/resources/img/Trash.png"/>
														</a>
													</td>
												</tr>
											</c:forEach>
										</c:if>
										</tbody>
									</table>
								</div>
							</div>
						</div>
					   <div style="margin:10px 0;"></div>
					   <div>
						   <input class="btn btn-primary"  onclick="up();" value=" 上移 ">
						   <input class="btn btn-primary"  onclick="down();" value=" 下移 ">
						   <input  class="btn btn-primary"  onclick="upFirst();" value=" 置顶 ">
						   <input class="btn btn-primary" onclick="downLast();" value=" 置底">
					   </div>
						<%--</html:form>--%>
					</form:form>
				<%--</div>
		</div>--%>
	</body>
</html>
