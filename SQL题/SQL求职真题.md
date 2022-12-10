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