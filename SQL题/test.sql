WITH user AS
(
	SELECT  DATE_FORMAT(start_time,'%Y%m') AS start_month
	       ,uid
	FROM exam_record
	GROUP BY  DATE_FORMAT(start_time,'%Y%m')
	         ,uid
), new_user AS
(
	SELECT  start_month
	       ,uid
	FROM user d1
	LEFT JOIN user d2
	ON d1.uid = d2.uid AND d1.start_month > d2.start_month
	WHERE d2.uid IS NULL 
)
SELECT  s1.start_month
       ,s1.mau
       ,s2.month_add_uv
       ,s2.max_month_add_uv
       ,s2.cum_sum_uv
FROM
(
	SELECT  start_month
	       ,COUNT(DISTINCT uid) AS mau
	FROM user
	GROUP BY  start_month
) s1
JOIN
(
	SELECT  start_month
	       ,COUNT(DISTINCT uid)                                   AS month_add_uv
	       ,MAX(COUNT(DISTINCT uid)) OVER( ORDER BY start_month ) AS max_month_add_uv
	       ,SUM(COUNT(DISTINCT uid)) OVER( ORDER BY start_month ) AS cum_sum_uv
	FROM new_user
	GROUP BY  start_month
) s2
ON s1.start_month = s2.start_month
ORDER BY start_month;