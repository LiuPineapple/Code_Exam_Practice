# SQL求职真题

## 社招202210
&nbsp;
### 20221207 百度-基础平台-一面
题目：一张用户登录记录表user_log，有user_id和log_time两个字段，要求找出连续登录超过3天的用户id
```sql
-- 方法一：使用偏移窗口函数
SELECT  user_id
FROM
(
	SELECT  user_id
	       ,DATE_FORMAT(log_time,'%Y%m%d') AS ds
	       ,LEAD(DATE_FORMAT(log_time,'%Y%m%d'),2) OVER(PARTITION BY user_id ORDER BY DATE_FORMAT(log_time,'%Y%m%d')) AS ds_2
	FROM user_log
	GROUP BY  user_id
	         ,DATE_FORMAT(log_time,'%Y%m%d')
)
WHERE DATADIFF(ds_2, ds, 'd') = 2
GROUP BY  user_id;

-- 方法二：使用排序窗口函数
SELECT  user_id
FROM
(
	SELECT  user_id
	       ,DATE_ADD(ds,-RANKS,'dd')
	       ,COUNT(1) AS nums
	FROM
	(
		SELECT  user_id
		       ,DATE_FORMAT(log_time,'%Y%m%d')                                                  AS ds
		       ,ROW_NUMBER() OVER(PARTITION BY user_id ORDER BY DATE_FORMAT(log_time,'%Y%m%d')) AS ranks
		FROM user_log
		GROUP BY  user_id
		         ,DATE_FORMAT(log_time,'%Y%m%d')
	)
	GROUP BY  user_id
	         ,DATE_ADD(ds,-RANKS,'dd')
)
WHERE nums >= 3
GROUP BY  user_id;
```

&nbsp;
### 20221208 快手-电商生态-二面
题目：一张商店售卖记录表，计算2022年每个月，每家店铺累积到当月的购买金额
```sql
-- 方法一：使用窗口函数
SELECT  shop_id
       ,DATE_FORMAT(sell_time,'%Y%m')                                                   AS the_month
       ,SUM(SUM(amt)) OVER(PARTITION BY shop_id ORDER BY DATE_FORMAT(sell_time,'%Y%m')) AS gmv
FROM shop_sell
WHERE year(sell_time) = '2022'
GROUP BY  shop_id
         ,DATE_FORMAT(sell_time,'%Y%m');

-- 方法二：使用两个子查询join 
SELECT  p1.shop_id
       ,p1.the_month
       ,SUM(p2.gmv) AS gmv
FROM
(
	SELECT  shop_id
	       ,DATE_FORMAT(sell_time,'%Y%m') AS the_month AS the_month
	       ,SUM(amt)                                   AS gmv
	FROM shop_sell
	WHERE year(sell_time) = '2022'
	GROUP BY  shop_id
	         ,DATE_FORMAT(sell_time,'%Y%m')
) p1
JOIN
(
	SELECT  shop_id
	       ,DATE_FORMAT(sell_time,'%Y%m') AS the_month AS the_month
	       ,SUM(amt)                                   AS gmv
	FROM shop_sell
	WHERE year(sell_time) = '2022'
	GROUP BY  shop_id
	         ,DATE_FORMAT(sell_time,'%Y%m')
) p2
ON p1.shop_id = p2.shop_id AND p1.the_month >= p2.the_month
GROUP BY  p1.shop_id
        ,p1.the_month;
```


&nbsp;
### 20230105 字节-抖音生活服务-一面
题目1：一张流量表，求用户近30、90、365天的登录天数

回答：
1. 模型解决：我们可以新建一张表。这张表有两个列，分别是user_id和log_date。建立一个每天都运行一次的任务，作用是每天取流量表中当天的登录用户user_id以及他的登录日期也就是当天，然后拿这个增量的数据跟刚才建立的那张表昨天的分区合并到一起，昨天的分区里面是以昨天为视角，看过去365天的登录用户及登录日期。合并之前先对昨天的分区做一个筛选，剔除log_date在365天之前的数据。合并之后的结果作为今天新的分区。那如果要求365天登录天数，只要看这张表的最新分区中对应user_id有几行就可以了。如果要看近30天登录天数，只要对于这张表的最新分区做一个log_date在近30天的筛选就可以了。
2. 模型解决：刚才说的是一个user_id如果有多个登录日期就对应多行数据的情况。另一种情况是，一个user_id对应一行数据，如果有多个登录日期，就放在一个数组当中。我们可以新建一张表。这张表有两个列，分别是user_id和log_date_array。建立一个每天都运行一次的任务，作用是每天取流量表中当天的登录用户user_id以及他的登录日期也就是当天，然后拿这个增量的数据跟刚才建立的那张表昨天的分区合并到一起。合并的时候要先对昨天分区中的数据做一个列转行的操作，也就是把一个array拆成多行，然后剔除掉log_date在365天之前的数据，然后再跟今天的增量数据合并到一起，然后再根据user_id做group by，把同一个user_id的登录日期重新拼到一个数组里，放到今天的最新分区中。如果仅仅只是解决这一个问题的话，第一种方法是更好的，因为第二种方法平白增加了行列互转的计算消耗。但是如果我们不想单独为这个求365天登录的事情就单独建立一张表，那我们就可以用第二种方法，把这个登录天数的数组放在用户画像表当中，一个user_id可能对应好多列，有年龄列，购买力列，还有近365天登录日期数组。

题目2：一张流量表，求近七天登录间隔少于24小时的用户。

回答：
1. sql解决：一次性取七天的数据，然后用窗口函数
2. 模型解决：我们可以新建一张表。这张表有两个列，分别是user_id和log_datetime。每天拿这张表昨天的分区和今天的增量数据去合并。今天的增量数据是指。我们拿今天和昨天两天的流量数据去做lead窗口函数的计算，找到昨天和今天两天登录间隔少于24小时，且第二次登录在今天的用户。之所以看两天是因为24小时最多不会超过两天的范围。这里可以举一个例子。把增量数据和昨天的分区合并到一起，合并之前先剔除昨天分区中距离今天超过7天的数据。这里的合并用UNION ALL就可以了。

&nbsp;
### 20230106 百度-电商技术-一面
连续登录天数和最大登录天数
https://blog.csdn.net/cptbtptp0825/article/details/120881178
行列互转